#define DISALLOW_COPY(classname) \
  classname(const classname &) = delete;\
  classname& operator=(const classname &) = delete;
