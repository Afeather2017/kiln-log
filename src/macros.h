#define DISALLOW_COPY(classname) \
  classname(const classname &) = delete;\
  classname& operator=(const classname &) = delete;
#define DISALLOW_COPYMOVE(classname) \
  classname(const classname &) = delete;\
  classname& operator=(const classname &) = delete; \
  classname(classname &&) = delete;\
  classname&& operator=(classname &&) = delete;
