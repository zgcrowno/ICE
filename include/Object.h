/**
 * @file Object.h
 * @date 13-Oct-2020
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "ICE.h"

/** Object Class
 */
class Object : public ScrollObject
{
public:


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &Info);


private:
};

#endif // __OBJECT_H__
