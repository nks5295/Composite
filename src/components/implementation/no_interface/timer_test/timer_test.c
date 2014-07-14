#include <cos_component.h>
#include <cos_debug.h>
#include <print.h>
#include <sched.h>
#include <res_spec.h>
#include "../../sched/cos_sched_sync.h"
#include "../../sched/cos_sched_ds.h"

extern int
parent_sched_child_timer_int(spdid_t spdid, int idle, unsigned long wake_diff);

extern int
parent_sched_child_cntl_thd(spdid_t spdid);

extern int
parent_sched_child_thd_crt(spdid_t spdid, spdid_t dest_spd);

extern void
parent_sched_exit(void);

void
cos_init(void)
{
	static volatile int first = 1, second = 1, spin = 0, count = 0, i = 0;
        int tid = 0;
        int ret = 0;

        printc("Prio: %d\n", sched_priority(cos_get_thd_id()));
        printc("TID: %d\n", cos_get_thd_id());

	if (first) {
		first = 0;
		union sched_param sp;
		sp.c.type = SCHEDP_PRIO;
		sp.c.value = 15;
                if (sched_create_thd(cos_spd_id(), sp.v, 0, 0) == 0) BUG();
		printc("!!!!!!!!!!!!Thread #%d\n", (int) cos_get_thd_id());
                return;

        } else if (second) {
                second = 0;
                printc("Calling cntl_thd\n");
                if (parent_sched_child_cntl_thd(cos_spd_id())) BUG();
                if (cos_sched_cntl(COS_SCHED_EVT_REGION, 0, (long)PERCPU_GET(cos_sched_notifications))) BUG();
		printc("Called cntl_thd\n");
                if ((tid = parent_sched_child_thd_crt(cos_spd_id(), cos_spd_id())) == -1) BUG();
                printc("THID = %d\n", tid);
                if (parent_sched_child_timer_int(cos_spd_id(), 0, 1) != 0) BUG();
                if ((ret = cos_switch_thread(tid, 0))) printc("Switch: %d\n", ret);

                if (parent_sched_child_timer_int(cos_spd_id(), 0, 1) != 0) BUG();
                if ((ret = cos_switch_thread(tid, 0))) printc("Switch: %d\n", ret);

                while (1) {
                        i++;
                        if (i % 100000000 == 0) {
                                printc("Stutter\n");
                        }
                }
                return;
	} else {
                while (1) {
                        spin++;
                        if (spin % 100000000 == 0) {
                                printc("f\n");
                        }
                }
                return;
        }
        printc("I finished\n");
}
