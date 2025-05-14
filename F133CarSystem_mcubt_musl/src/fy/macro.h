#ifndef _FY_MACRO_H_
#define _FY_MACRO_H_

#define DISALLOW_COPY_AND_ASSIGN(classname) \
private:                                   \
    classname(const classname &);             \
    classname &operator=(const classname &);


#define DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
private:                                         \
    classname();                                 \
    DISALLOW_COPY_AND_ASSIGN(classname);


#define DECLARE_SINGLETON(classname)        \
public:                                    \
    static classname *instance() {            \
    static classname singleton;              \
    return &singleton;                       \
}                                         \
DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
    private:

#endif
