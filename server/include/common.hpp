#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#define DISABLE_MOVE_CONSTRUCTOR(classname) \
    classname(classname&&) = delete;\
    classname& operator=(classname&&) = delete;

#define DISABLE_COPY_CONSTRUCTOR(classname) \
    classname(const classname&) = delete;\
    classname& operator=(const classname&) = delete;

namespace ZDC
{
    
} // namespace ZDC
#endif//__COMMON_HPP__ GUARD