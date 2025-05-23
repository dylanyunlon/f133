#ifndef _FY_SIMPLE_FORM_
#define _FY_SIMPLE_FORM_

#include <string>
#include <vector>

#include "control/ZKListView.h"

namespace fy {

class simple_form {
public:
  typedef std::string (*string_getter)(int index, void* user_data);
  typedef void (*click_listener)(int index, void* user_data);

  struct form_item {
    std::string title;
    string_getter get_value;
    click_listener on_click;
    void* user_data;
  };

  void add_item(const std::string &title, string_getter get_value, click_listener on_click, void* user_data) {
    form_item item;
    item.title = title;
    item.get_value = get_value;
    item.on_click = on_click;
    item.user_data = user_data;
    items_.push_back(item);
  }

  int size() { return items_.size();}

  void clear() { items_.clear();}

  const form_item* find(int index) {
    if (index >= (int)items_.size()) {
      return NULL;
    }
    return (form_item*)&(*(items_.begin() + index));
  }

  void obtain(ZKListView::ZKListItem *pListItem, int index, int subitem_id) {
    const form_item* item = find(index);
    if (item == NULL) {
      return;
    }
    if (pListItem) {
      pListItem->setText(item->title);
    }
    ZKListView::ZKListSubItem* value = pListItem->findSubItemByID(subitem_id);
    if (value == NULL) {
      return;
    }
    if (item->get_value == NULL) {
      value->setText("");
      return;
    }
    value->setText(item->get_value(index, item->user_data));
  }

  void click(int index) {
    const form_item* item = find(index);
    if (item == NULL) {
      return;
    }
    if (item->on_click == NULL) {
      return;
    }
    item->on_click(index, item->user_data);
  }

private:
  std::vector<simple_form::form_item> items_;
};


} /* namespace fy */

#endif
