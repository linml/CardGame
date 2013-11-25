//
//  gameMiddle.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-9-4.
//
//

#include "gameMiddle.h"
#import <MessageUI/MessageUI.h>
#include <string>

namespace Middle {
    void showAlertView(const char* pchMsgData)
    {
        UIAlertView* v = [[UIAlertView alloc] initWithTitle:@"Info" message:[NSString stringWithUTF8String:pchMsgData] delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [v show];
        [v release];
    }
    
    string getAppDocumentPath()
    {
        // save to document folder
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        std::string strRet = [documentsDirectory UTF8String];
        strRet.append("/");
        return strRet;
    }

}