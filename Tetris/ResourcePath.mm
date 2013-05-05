//
//  ResourcePath.mm
//
//  Created by Cam Allen on 11/10/12.
//

#import <Cocoa/Cocoa.h>
#import "ResourcePath.h"

std::string ResourcePath(const char *Name)
{
    return [[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:Name] ofType:nil] UTF8String];
}

std::string ResourcePath(const std::string Name)
{
    return ResourcePath(Name.c_str());
}