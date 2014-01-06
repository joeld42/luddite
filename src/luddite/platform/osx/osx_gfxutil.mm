

#include <luddite/platform/platform.h>

#import <GLKit/GLKTextureLoader.h>

uint32_t pfLoadTexture( const char *filename )
{

    NSString *fnstring = [NSString stringWithUTF8String:filename];
    NSString *fnExt = [fnstring pathExtension];
    NSString *fnFile = [fnstring stringByDeletingPathExtension];
    
    NSString *textureFile = [[NSBundle mainBundle] pathForResource: fnFile ofType:fnExt ];
    
    NSLog( @"pfLoadTexture:   file '%@' ext '%@' will load %@", fnFile, fnExt, textureFile );
    
    NSError *error = nil;
    GLKTextureInfo *texture = [GLKTextureLoader textureWithContentsOfFile: textureFile
                                                                  options:nil error:&error ];
    if (error) {
        NSLog( @"Error loading texture '%s': %@", filename, error );
        return 0;
    }
    
    glBindTexture(GL_TEXTURE_2D, texture.name );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );
    
    return texture.name;  
}


eastl::string pfPathToResource( const char *resource )
{
    NSString *fnstring = [NSString stringWithUTF8String:resource ];
    NSString *fnExt = [fnstring pathExtension];
    NSString *fnFile = [fnstring stringByDeletingPathExtension];
    
    NSString *resourceFile = [[NSBundle mainBundle] pathForResource: fnFile ofType:fnExt ];
    
    return eastl::string( [resourceFile UTF8String]);
}

eastl::string pfGetResourcePath()
{
    eastl::string resourcePath;
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef bundleURL = CFBundleCopyBundleURL(mainBundle);
	CFStringRef str = CFURLCopyFileSystemPath( bundleURL, kCFURLPOSIXPathStyle );
	CFRelease(bundleURL);
	char path[PATH_MAX];
	
	CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
	CFRelease(str);
    
    resourcePath = path;
    
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	str = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle );
	CFRelease(resourcesURL);
    
    CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
	CFRelease(str);
    
    resourcePath = resourcePath + "/" + path;

    return resourcePath;
}