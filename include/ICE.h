/**
 * @file ICE.h
 * @date 13-Oct-2020
 */

#ifndef __ICE_H__
#define __ICE_H__

#define __NO_SCROLLED__
#include "Scroll.h"
#include <vector>

/** Game Class
 */
class ICE : public Scroll<ICE>
{
public:
    std::vector<ScrollObject*> GetPlayerProjectiles() const;
private:
                static orxSTATUS orxFASTCALL EventHandler(const orxEVENT* _pstEvent);

                orxSTATUS       Bootstrap() const;

                void            Update(const orxCLOCK_INFO &Info);

                orxSTATUS       Init();
                orxSTATUS       Run();
                void            Exit();
                void            BindObjects();


private:
};

#endif // __ICE_H__
