//
//  gameMiddle.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-9-4.
//
//

#include "gameMiddle.h"
#import <MessageUI/MessageUI.h>

namespace Middle {
    void showAlertView(const char* pchMsgData)
    {
        UIAlertView* v = [[UIAlertView alloc] initWithTitle:@"Info" message:[NSString stringWithUTF8String:pchMsgData] delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [v show];
        [v release];
    }
}