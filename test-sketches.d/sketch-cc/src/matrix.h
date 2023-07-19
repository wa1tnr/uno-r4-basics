// matrix.h

#define write_register() push(clr); acld(); push(setb); asbd();

NAMED(_l1d, "l1"); void l1d() { int clr = 3; int setb = 7; write_register(); }
NAMED(_l2d, "l2"); void l2d() { int clr = 7; int setb = 3; push(clr);  acld(); push(setb); asbd(); }
NAMED(_l3d, "l3"); void l3d() { int clr = 4; int setb = 7; push(clr);  acld(); push(setb); asbd(); }
NAMED(_l4d, "l4"); void l4d() { int clr = 7; int setb = 4; push(clr);  acld(); push(setb); asbd(); }
NAMED(_l5d, "l5"); void l5d() { int clr = 4; int setb = 3; push(clr);  acld(); push(setb); asbd(); }
NAMED(_l6d, "l6"); void l6d() { int clr = 3; int setb = 4; push(clr);  acld(); push(setb); asbd(); }

/* END */
