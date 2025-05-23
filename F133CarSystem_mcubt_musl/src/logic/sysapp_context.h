/*
 * sysapp_context.h
 *
 *  Created on: 2023年5月26日
 *      Author: ZKSWE Develop Team
 */

#ifndef _LOGIC_SYSAPP_CONTEXT_H_
#define _LOGIC_SYSAPP_CONTEXT_H_

typedef void (*app_register_timer_fun)(int id, int time);
typedef void (*app_unregister_timer_fun)(int id);

namespace app {

void attach_timer(app_register_timer_fun reg, app_unregister_timer_fun unreg);
bool on_timer(int id);

void show_ctrlbar();
void hide_ctrlbar();
bool is_show_ctrlbar();
bool is_hit_ctrlbar(int x, int y);

void show_topbar();
void hide_topbar();
bool is_show_topbar();
bool is_hit_topbar(int x, int y);

LayoutPosition get_topbar_position();
}

#endif /* _LOGIC_SYSAPP_CONTEXT_H_ */
