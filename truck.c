/*

    Copyright (c) 2011 by Keith M. Knowles.
    Free software licensed under the terms of the
    GNU Lesser General Public License.


    Program:		Truck -- B-double truck simulator
    Module:		truck.c -- simulation model
    Version:		1
    Author:		Keith M. Knowles.
    Date:		11/10/08


    2D plan view simulation of a B-double truck for the practice of
    reversing.


    Changes:

    11/10/08 kmk	Initial version.


    Notes:


    Tasks:

      . (see #coupling)
      . (see #engine gear/rpm management)


    Description:

    Define the model and advance it increment by increment.  The main
    routine will read a file, (see truck.txt), of commands that control the
    simulation.

    Commands are:

    A  -- couple/decouple the A trailer
    B  -- couple/decouple the B trailer
    C  -- center the steering wheels
    G  -- click the Gear Selector
    Ln -- steer left
    Q  -- shut down the simulation
    Rn -- steer right
    T  -- toggle Trigger

    Our objective is to start with tractor and trailers decoupled and
    separate: tt, aaaa, bbbbbbbb.  Couple them up and then reverse them
    through a gate and out onto a street, TTAAAABBBBBBBB:

					|	|
					|   B	|
					|   B	|
					|   B	|
					|   B	|
					|   B	|
					|   B	|
					|   B	|
					|   B	|
			    tt		|   A	|
					|   A	|
			    aaaa	|   A	|
					|   A	|
			    bbbbbbbb	|   T	|
					    T	|
						|
						|
						|
					|	|
					|	|
*/

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#undef KMK
#ifdef KMK

# include <std/c.h>
# include <memory/heap/heap.h>
# include <locus/v2/locus.h>

#else

# include <stdlib.h>
# include <malloc.h>
# include <string.h>
# include <c.h>
# define New(sz)	malloc(sz)
# define New0(sz)	( pvt_temp =  malloc(sz),			\
			  memset(pvt_temp, 0, (sz)), pvt_temp )
  static pvt	pvt_temp;
# define Loc_module

#endif

#define	Cmd_buf_sz	65536
#define	Ticks		100		// simulation increment, Hz

#define	Pi		3.1415926535897932384626
#define	Dir_west	270

#define	Point(pt)	((Point *)(pt))
typedef struct Point Point;
struct Point {
  double	x, y;
};

/*  Use degrees, 0..360, for compass bearing and convert to radians for
    trig, using (90 - deg) * Pi / 180.
*/
#define	Vehicle(veh)	((Vehicle *)(veh))
typedef struct Vehicle Vehicle;
typedef enum Gear Gear;
struct Vehicle {
  Vehicle *	coupled;		// following trailer, if any
  double	hdng;			// compass heading (degrees)
  Point		pos;
  double	wheel_base;		// dist between front/rear wheels

  // following apply to tractors only
  int		brake;			// 0..5
  double	cruise;			// 0 => not set
  struct {
    enum Gear {
      Gear_N,
      Gear_1, Gear_2, Gear_3, Gear_4,
      Gear_5, Gear_6, Gear_7, Gear_8,
      Gear_max = Gear_8,
      Gear_R2 = -2, Gear_R1,
      Gear_min = Gear_R2
    }		engaged;
    Gear	selected;		// preselection
    bool	clutch;			// engine transmitting
  }		gear;
  bool		park;			// park brake applied
  bool		power;			// power applied
  double	speed;
  int		steer;			// in degrees relative to hdng
  int		rpm;			// indicates engine running or not
};

#define	Model(model)	((Model *)(model))
typedef struct Model Model;
struct Model {
  int		time;			// in ticks, (0.01 seconds)
  Vehicle *	tractor;
  Vehicle *	trailer_A;
  Vehicle *	trailer_B;
};

#define	Control(ctl)	((Control *)(ctl))
typedef struct Control Control;
struct Control {
  int		time;			// next sample time
  enum {
    Cmd_A, Cmd_B, Cmd_C, Cmd_G, Cmd_Q, Cmd_T
  }		attn;			// attention mask
  struct {
    uchar	buf[Cmd_buf_sz];
    uchar *	ptr;
  }		cmds;
  Model *	model;
  bool		gear_up;		// gear selection up, not down
  struct {
    bool	cruise;			// state of cruise
    int		since;			// time of last transition on
    bool	state;
  }		trigger;
  int		turn;			// -9..9
};

void		model_advance(Model *model);
Model *		model_init(void);
void		plot_display(void);
void		plot_init(void);
void		plot_point(Point *pt);
void		vehicle_init(Model *);

static struct {
# define	Plot_max	200	// 0..200
  char		board[Plot_max + 1][Plot_max + 1];
  int		x_min, x_max;
  int		y_min, y_max;
}		plot;

/*  Target speeds for each gear at 1000 rpm.

    [111014] update, from observation of Hino GH:

	Hino	GH:
    gear rpm	speed		@1000rpm	@1400 rpm
    1				4.76		6.67
    2	1500	10		6.67		9.33
    3				9.33		13.07
    4	1600	20		12.50		17.50
    5				17.50		24.50
    6	1500	35		23.33		32.67
    7				32.67		45.73
    8	1200	50		41.67		58.33
    8	2400	100		[is this right?]

    We are going to include a form of cruise control to gain precise control
    of speed; e.g. in a 25 kmh roadwork area.  Hold the throttle, then
    release and click the throttle.

    Target, nice speed is at 1200 rpm; G8 is an exception(?).

    [111014] we need an indication of acceleration possible in each gear.
*/
double		speeds[] = {		// @ 1000 rpm
  -5,					// R2
  -1,					// R1
   0,					// N
   4.76,				// 1
   6.67,				// 2
   9.33,				// 3
  12.50,				// 4
  17.50,				// 5
  23.33,				// 6
  32.67,				// 7
  41.67,				// 8
};

double		accel[] = {		// indicated in rpm/sec
   500,					// R2
   800,					// R1
   0,					// N
   800,					// 1
   600,					// 2
   400,					// 3
   300,					// 4
   200,					// 5
   150,					// 6
   120,					// 7
   100,					// 8
};

Loc_module;

/*  (tag control_execute)

    Interprets and applies the control information on each clock tick.
*/
void control_execute(Control *ctl) {
  Vehicle *tractor = ctl->model->tractor;

  if (ctl->attn & Bit(Cmd_A)) {
    printf("%4d: A unimpl!\n", ctl->model->time);
  }

  if (ctl->attn & Bit(Cmd_B)) {
    printf("%4d: B unimpl!\n", ctl->model->time);
  }

  if (ctl->attn & Bit(Cmd_C)) {		// incorporates also the L & R cmds
    tractor->steer = ctl->turn * 5;
    printf(
      "%4d: %c%d is %d degrees\n",
      ctl->model->time,
      ctl->turn ? (ctl->turn < 0 ? 'L' : 'R') : 'C',
      ctl->turn >= 0 ? ctl->turn : -ctl->turn,
      tractor->steer);
  }

  if (ctl->attn & Bit(Cmd_G)) {
    ctl->trigger.since = ctl->model->time;
    printf(
      "%4d: G %s T\n",
      ctl->model->time,
      ctl->gear_up ? "and" : "no");
    // enforce speed=0 in order to shift out of N.
    if (tractor->gear.engaged != Gear_N || !tractor->speed) {
      if (tractor->cruise) printf("%4d: cruise canceled.\n", ctl->model->time);
      tractor->cruise = 0;		// cancel cruise
      if (ctl->gear_up) {
	// shift up gear
	if (tractor->gear.engaged >= 0) {
	  if (tractor->gear.engaged < Gear_max) tractor->gear.engaged++;
	} else if (tractor->gear.engaged > Gear_min) {
	  tractor->gear.engaged--;
	}
	printf("%4d: upshift to %d.\n", ctl->model->time, tractor->gear.engaged);
      } else {
	// shift down gear
	if (tractor->gear.engaged >= 0) {
	  tractor->gear.engaged--;
	} else {
	  tractor->gear.engaged++;
	}
	printf(
	  "%4d: %sshift to %d.\n",
	  ctl->model->time,
	  tractor->gear.engaged == Gear_R1 ? "up" : "down",
	  tractor->gear.engaged);
      }
    }
  }

  if (ctl->attn & Bit(Cmd_Q)) {
    printf("* simulation shut-down!\n");
    plot_display();
    exit(0);
  }

  if (ctl->attn & Bit(Cmd_T)) {
    int duration = ctl->model->time - ctl->trigger.since;
    printf(
      ctl->trigger.state ?
        "%4d: T held\n" :
        "%4d: T released after %d, (since %d)\n",
      ctl->model->time,
      duration,
      ctl->trigger.since);
    if (!tractor->rpm) {
      // engine start
      tractor->rpm = 100;		// turn over the engine
      printf("%4d: cranking...\n", ctl->model->time);
    } else if (tractor->rpm < 200) {	// cranking
      // engine started?
      if (!ctl->trigger.state) {
	if (duration > 15) {
	  tractor->rpm = 600;		// engine has started, now idling
	  printf("%4d: engine started: congratulations!!\n", ctl->model->time);
	} else {
	  tractor->rpm = 0;		// not long enough to start
	  printf("%4d: no start!!\n", ctl->model->time);
	}
      }
    } else if (tractor->park) {
      if (!ctl->trigger.state) {
	if (duration >= 10) {
	  // engine stop
	  tractor->rpm = 0;
	  printf("%4d: engine stopped.\n", ctl->model->time);
	} else if (duration <= 2) {
	  // park brake release
	  tractor->brake = 1;		// enough to hold still on the flat
	  tractor->park = false;
	  printf("%4d: Park brake released.\n", ctl->model->time);
	}
      }
    } else if (tractor->gear.engaged == Gear_N) {
      // park brake set
      if (!ctl->trigger.state) {
	if (duration >= 15) {
	  printf("%4d: Park brake set.\n", ctl->model->time);
	  tractor->park = true;
	} else {
	  printf("%4d: power off.\n", ctl->model->time);
	}
      }
    } else {				// in gear
      /*  (tag engine)

          We will display the gear selected.  It will be grey when de-
	  clutched.  A postfix indicator will use an up- or down-arrow to
	  indicate whether we have pre-selected another gear, higher or
	  lower.

	  When in the correct gear for the speed, and not down-shifted, the
	  clutch will be active and the rpm will track the speed for the
	  gear.

	  Down-shifting will apply brakes and the rpm will track the speed
	  until the gear shifts down; it will then rise up to meet the new
	  target, declutched, and then reclutch.  (This is consistent with
	  implementation of engine-braking.)

	  Power on simply causes thrust to be applied and the rpm presumably
	  to increase, (sinusoidally).  Driver shifts up allowing a change
	  of gear.  Gears may be skipped if the acceleration is hard enough
	  and rpm range permits.

	  In the event that power is applied while declutched, the rpm must
	  rise to the target before reclutching.
      *///
      if (ctl->trigger.state) {
	// apply power
	printf("%4d: power on.\n", ctl->model->time);
	tractor->power = true;
	tractor->cruise = tractor->cruise; // save cruise state
      } else {
	// release power
	printf("%4d: power off.\n", ctl->model->time);
	tractor->power = false;
	// if this was in short enough time, toggle cruise control
	if (duration <= 15) {
	  tractor->cruise = !tractor->cruise;
	  printf(
	    "%4d: cruise %s\n",
	    ctl->model->time,
	    tractor->cruise ? "on" : "off");
	}
      }
    }
  }

  ctl->attn = 0;
}

/*  Open the control command file and read in the whole contents.
*/
Control *control_init(Model *model, char *ctl_file_name) {
  Control *ctl = New(sizeof(Control));
  ctl->model = model;
  int cmd_file = open(ctl_file_name, O_RDONLY);
  if (!cmd_file) {
    printf("* cmd %s open fail!\n", ctl_file_name);
    exit(0);
  }
  int len = read(cmd_file, ctl->cmds.buf, sizeof(ctl->cmds.buf) - 1);
  if (len < 0) {
    printf("* cmd read fail!\n");
    exit(0);
  }
  if (len == Cmd_buf_sz) {
    printf("* cmd buffer too small!\n");
    exit(0);
  }
  close(cmd_file);
  ctl->cmds.buf[len] = '\0';	// sentinel
  ctl->cmds.ptr = ctl->cmds.buf;
  ctl->time = 0;
  return ctl;
}

/*  Once  processing for time T  has been completed,  call control_sample in
    order to get  the next packet of signals occurring at  some future tick. 
    That time will be indicated in the control state along with all relevant
    signals.

    Generally, mutually-exclusive  events are forced to be  separated by one
    tick.  In  addition, an explicit delay  can be indicated  in the command
    file.

    For simplicity, let's  assert that there may be only one  of any kind of
    event.  So, only one of the L, R or C commands; only one G command; only
    one of A and B.  We could say only one of T, but the sequence T t, (hold
    and release the trigger, i.e. click it), should be permissible.

    Returns false at end of file.

    Think we  should make ctl->time the  time of next  expected input.  Call
    before then and nothing happens.  Call on that time and we parse what we
    can and then set the next expected time.

    How about a command mask: ABCGLRT with turn angle and trigger duration?
*/
bool control_sample(Control *ctl) {
  uchar *	cp = ctl->cmds.ptr;
  int		delay;
  bool		left = false;
  int		turn;

  ctl->attn = 0;
  if (ctl->model->time < ctl->time) return true; // not yet

  do {					// scan lines for significant token
    // scan whitespace
    while (*cp == ' ' || *cp == '\t' || *cp == '\r' || *cp == '\n') cp++;
    ctl->cmds.ptr = cp;			// checkpoint
    switch (*cp) {
    case '!':				// comment
      do cp++; while (*cp != '\n');	// skip to eol
      break;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      delay = 0;
      do {
	delay = 10 * delay + *cp++ - '0';
      } while (*cp >= '0' && *cp <= '9');
      ctl->cmds.ptr = cp + 1;
      if (delay) {
	ctl->time = ctl->model->time + 10 * delay;
	return true;
      }
      break;
    case 'A': case 'a':			// couple/decouple A-trailer
      /// (tag coupling)
      if (ctl->attn & (Bit(Cmd_A) | Bit(Cmd_B))) {
	ctl->time = ctl->model->time + 10;
	return true;
      }
      ctl->attn |= Bit(Cmd_A);
      break;
    case 'B': case 'b':			// couple/decouple B-trailer
      if (ctl->attn & (Bit(Cmd_A) | Bit(Cmd_B))) {
	ctl->time = ctl->model->time + 10;
	return true;
      }
      ctl->attn |= Bit(Cmd_B);
      break;
    case 'C': case 'c':			// center the wheels
      if (ctl->attn & Bit(Cmd_C)) {
	ctl->time = ctl->model->time + 10;
	return true;
      }
      ctl->attn |= Bit(Cmd_C);		// indicate change in turn
      ctl->turn = 0;
      printf("\tset turn to %d\n", ctl->turn);
      break;
    case 'G': case 'g':			// click gear selector
      if (ctl->attn & Bit(Cmd_G)) {
	ctl->time = ctl->model->time + 10;
	// this prevents the Park brake getting set if we are going into N.
	ctl->trigger.since = ctl->model->time;
	return true;
      }
      ctl->attn |= Bit(Cmd_G);
      ctl->gear_up = ctl->trigger.state;
      break;
    case 'L': case 'l':			// turn left
      left = true;
    case 'R': case 'r':			// turn right
      if (ctl->attn & Bit(Cmd_C)) {
	ctl->time = ctl->model->time + 10;
	return true;
      }
      ctl->attn |= Bit(Cmd_C);		// indicate change in turn

      turn = 0;
      cp++;
      while (*cp >= '0' && *cp <= '9') {
	turn = 10 * turn + *cp++ - '0';
      }
      cp--;
      ctl->turn = left ? -turn : turn;
      break;

    case 'Q': case 'q':
      ctl->attn |= Bit(Cmd_Q);
      break;

    /*  This is the interesting one.

        When the truck is stationary and in neutral:

	  - a short click releases the Park brakes.
	  - a long hold applies the Park brakes, if not already.
	  - a long hold stops the engine, if the Park brakes are applied and
	    the engine is running.
	  - a long hold starts the engine, if it is stopped.
    */
    case 'T': case 't':			// trigger throttle
      if (!ctl->trigger.state && ctl->attn & Bit(Cmd_T)) {
	ctl->time = ctl->model->time + 10;
	return true;
      }
      if (ctl->trigger.state = !ctl->trigger.state) {
	ctl->trigger.since = ctl->model->time;
      }
      ctl->attn |= Bit(Cmd_T);		// no more T commands
      break;
    case '\0':				// eof
      ctl->cmds.ptr = cp;		// freeze at end of file
      ctl->time = Infinity;
      return false;
    }
    cp++;
  } while (true);
}

/*  Display as follows:

    tttt:  T:x,y h v rpmG B P  A:x,y h  B:x,y h
*/
int display_update(Model *model) {

  /*  Display the tractor.
  */
  Vehicle *tractor = model->tractor;
  printf(
    "%4d: --- %4d/%-4d %03d %.2f %c%d %4d%c",
    model->time,
    (int)tractor->pos.x,
    (int)tractor->pos.y,
    (int)tractor->hdng,
    tractor->speed,
    tractor->gear.engaged ? (tractor->gear.engaged < 0 ? 'R' : 'G') : 'N',
    tractor->gear.engaged < 0 ? -tractor->gear.engaged : tractor->gear.engaged,
    tractor->rpm,
    tractor->brake ? 'B' : ' ');

  /*  Display the A-trailer, if coupled.
  */
  if (tractor->coupled) {
    Vehicle *	trailer_A = tractor->coupled;

    /*  Display the B-trailer, if coupled.
    */
    if (trailer_A->coupled) {
      Vehicle *	trailer_B = trailer_A->coupled;

      ///
    }
  }

  printf(" ---\n");
  return model->time + 100;
}

int main(int argc, char *argv[]) {
  int		run_down = 20;		// 20 ticks after last cmd
  Model *	model = model_init();
  Control *	control;
  int		tim_display = 0;

  if (argc < 2) {
    printf("* usage: truck ctl_file_name!\n");
    return 0;
  }
  control = control_init(model, argv[1]);

  plot_init();

  /*  We want a loop that does the basic time advance on an interval that we
      use to  plot the  model update.  In  addition, we will  sample control
      inputs and update  the display; both of these on  some multiple of the
      basic model update increment.
  */
  bool ucmd = true;
  do {					// each cycle is one tick: 100 Hz
    if (control->time == model->time) ucmd = control_sample(control);
    if (control->attn) control_execute(control);
    model_advance(model);
    if (model->time >= tim_display || !ucmd) {
      tim_display = display_update(model);
    }
    if (!ucmd) run_down--;
  } while (run_down);
  plot_display();
}

/*  (tag model_advance)

    Advance the state of the model by one clock tick.

    Handle power, acceleration, braking and coasting.
    Handle translation due to speed.
*/
void model_advance(Model *model) {
  Vehicle *tractor = model->tractor;
  model->time++;

  /*  Handle power, acceleration, braking and coasting.

      For the selected  gear, there is a target speed.  We  are at, above or
      below that target  speed.  Power is on or off.   Brakes are applied in
      one of five degrees or not applied at all.

      If power  is off, brakes are  used to slow  down to the target  speed. 
      After that, speed decays gently.

      If power is  on, the truck accelerates to and holds  the target speed. 
      If the speed is greater than the target, speed coasts down gently.

	      Speed	Power:On	Off
	      -----	--------	---
		>T	 coast		brake
		=T	 hold		coast
		<T	 accel		coast

      [111014] 
  */
  int target = speeds[tractor->gear.engaged - Gear_min];

  /*  If power on: thrust is from 0 to target speed.
      If power off: thrust is from 0 to actual speed.
      Yet coast always has to be toward 0.
  */
  int thrust;

  if (tractor->gear.engaged != Gear_N && (tractor->power || tractor->cruise)) {
    thrust = tractor->gear.engaged > Gear_N ? 1 : (tractor->gear.engaged < Gear_N ? -1 : 1);
    /// calculate rpm from speed and gear
    /// we'll use an rpm limiter, or just scale thrust back.  for now, just
    /// get motion done.
    // apply acceleration
    if (!tractor->cruise || thrust * (tractor->cruise - tractor->speed) > 0) {
      tractor->speed += 3.0 / Ticks * thrust;
    }
  } else {
    thrust = tractor->speed > 0 ? 1 : -1;////(tractor->gear.engaged < Gear_N ? -1 : 0);
    tractor->brake = (target - tractor->speed) * thrust < 0;
    /// coast to zero or brake.
    tractor->speed -= (tractor->brake ? 6.0 : 0.5) / Ticks * thrust;
    if (tractor->speed * thrust < 0) tractor->speed = 0; // clip
  }

  /*  Handle translation due to speed.

      There are two  cases: straight-line motion and motion  around a curve. 
      (If we were careful, we might be able to combine the two.  Nope.)

      The length of  the wheelbase is w.  The angle  of the (front) steering
      wheels is alpha.  The turn radius, r, measured from the turn center to
      the steering  wheels is  r = w/sin(alpha).   These wheels  will follow
      this (outer) circle for an angle of arc given by v*t/r.  The radius of
      the inner circle that the rear  wheels follow is rr = r*cos(alpha) and
      they'll follow the same angle, of course.

      Alpha = 0  would cause a division by  zero but indicates straight-line
      travel.  (Let's just say that an angle within 0.1 degrees of 0 is 0.)

      The distance traveled around the circumference of those circles is:
      s = r.alpha
      which yields an actual rotation around the center of s/r.

      Calculate the center-point and then  rotate the front wheels around it
      by s/r and the rear wheels by s/rr.  Rotate using:

      x' = x.cos(alpha) - y.sin(alpha)
      y' = x.sin(alpha) + y.cos(alpha);

      Unhandled factors:
      - incline
      - air resistance
      - wind
      - surface: water/ice
      - load weight, trailers
      - centrifugal force while cornering
  */
  if (!tractor->steer) {		// travel in a straight line
    double cosx = cos((90 - tractor->hdng) * Pi / 180);
    double siny = sin((90 - tractor->hdng) * Pi / 180);
    tractor->pos.x += tractor->speed * cosx / Ticks;
    tractor->pos.y += tractor->speed * siny / Ticks;
  } else {				// travel around a circle
    /*  Angles are stored as compass bearings, 0..360, in degrees.

        But, for the trigonometry, we convert to radians expressed in a
	counter-clockwise direction.  (Hence, the negative sign.)

	In the diagram, truck.jpg, the wheels at F are turned to steer an
	angle of alpha, relative to the wheelbase, (the line FR).  The front
	(F) and rear (R) wheels circle a common center of rotation, C,
	although the radii, FC and RC, are of different lengths.  The angle
	made by the intersection of FC and RC at C is alpha, the same as the
	steering angle of the front wheels.
    */
    double alpha = -tractor->steer * Pi / 180;
    double sinr = sin(alpha);

    /*  w = rf.sin(alpha), (w: wheelbase, rf: outer radius) and thus:

        rf = w/sin(alpha), (rf: outer radius, to front wheels)
    */
    double rf = tractor->wheel_base / sinr;	// radius: front wheels

    /*  Once we have the outer radius, we can compute the inner radius to
	the rear wheels, R, as the product of the outer radius with the
	cosine of alpha.
    */
    double rr = rf * cos(alpha);		// radius: rear wheels

    /*  s = v.t, (s: distance, v:speed, t:time), and
	circumference = 2.pi.r.beta/(2.pi), which yields:
	circ = r.beta, (beta: angle traversed around the center)

	Therefore v.t = r.beta and beta = v.t/r
    */
    double beta = tractor->speed / Ticks / rf;	// angle traveled
    if (tractor->steer > 0) beta = -beta;

    /*  Find the center of rotation and the vector from it.

        Gamma is derived from the heading and is the direction of the center
        of rotation.

	When turning left, the center is 90 degrees to the left; otherwise,
	90 degrees to the right.
    */
    //double gamma = tractor->hdng + (tractor->steer > 0 ? 90 : -90);
    double gamma = tractor->hdng 
      + tractor->steer 
      + (tractor->steer > 0 ? 90 : -90);
    gamma = (90 - gamma) * Pi /180;

    Point cent;
    cent.x = tractor->pos.x + rf * cos(gamma);
    cent.y = tractor->pos.y + rf * sin(gamma);
    Point dist;
    dist.x = tractor->pos.x - cent.x;
    dist.y = tractor->pos.y - cent.y;

    /*  Rotate that vector by the angle of traversal, beta.

	Rotation is given by:
	x' = x.cosA - y.sinA
	y' = x.sinA + y.cosA
    */
    double cosb = cos(beta);
    double sinb = sin(beta);
    Point trans;
    trans.x = dist.x * cosb - dist.y * sinb;
    trans.y = dist.x * sinb + dist.y * cosb;
    tractor->pos.x = cent.x + trans.x;
    tractor->pos.y = cent.y + trans.y;
    tractor->hdng += (tractor->steer >= 0 ? beta : -beta) * 180 / Pi;
    if (tractor->hdng >= 360) tractor->hdng -= 360;
    else if (tractor->hdng < 0) tractor->hdng += 360;
    if (false) printf(
      "%4d: hdng outer %.1f inner %.1f beta %f (%.1f,%.1f) %.1f\n",
      model->time,
      rf,
      rr,
      beta * 180 / Pi,
      cent.x,
      cent.y,
      tractor->hdng);
  }
  plot_point(&tractor->pos);
}

Model *model_init(void) {
  Model *model = New0(sizeof(Model));
  vehicle_init(model);
  return model;
}

/*  This plot facility deserves enhancement and promotion to be a separate
    facility in its own right.  ////
*/
void plot_display(void) {
  int		y;

  for (y = plot.y_max; y >= plot.y_min; y--) {
    printf(
      "%4d: %.*s\n",
      y - Plot_max / 2,
      plot.x_max - plot.x_min,
      &plot.board[y][plot.x_min]);
  }
}

void plot_init(void) {
  memset(plot.board, ' ', sizeof(plot.board));
  plot.x_min = Plot_max;
  plot.x_max = 0;
  plot.y_min = Plot_max / 2;
  plot.y_min = Plot_max / 2;
}

void plot_point(Point *pt) {
  int x = 2 * pt->x;
  int y = pt->y;
  // clip
  if (x < 0) x = 0; else if (x > Plot_max) x = Plot_max;
  if (y < 0) y = 0; else if (y > Plot_max) y = Plot_max;
  plot.board[y][x] = '+';
  if (x < plot.x_min) plot.x_min = x;
  else if (x > plot.x_max) plot.x_max = x;
  if (y < plot.y_min) plot.y_min = y;
  else if (y > plot.y_max) plot.y_max = y;
}

/*  Tractor starts out uncoupled, in neutral, park brake applied, engine
    stopped.
*/
void vehicle_init(Model *model) {

  /*  Create the tractor.
  */
  Vehicle *veh = model->tractor = New0(sizeof(Vehicle));
  veh->pos.x = 100;
  veh->pos.y = 100;
  veh->speed = 0;
  veh->hdng = Dir_west;
  veh->park = true;			// park brakes set
  veh->wheel_base = 4;

  /*  Create the trailers.
  */
  veh = model->trailer_A = New0(sizeof(Vehicle));
  veh->pos.x = 100;
  veh->pos.y = 75;
  veh->hdng = Dir_west;

  veh = model->trailer_B = New0(sizeof(Vehicle));
  veh->pos.x = 100;
  veh->pos.y = 50;
  veh->hdng = Dir_west;
}
