

#import <OpenGLES/ES2/gl.h> 
#import <GLKit/GLKTextureLoader.h>

#import <luddite/common/debug.h>

#import "platform.h"

using namespace luddite;

// kind of a mess putting this here, this is kind of a temporary until
// there's a more complete texture or resource management in place. But maybe
// I can get away without it.. usually more trouble than it's worth
uint32_t pfLoadTexture( const char *filename )
{
    NSString *fnstring = [NSString stringWithUTF8String:filename];
    NSString *fnExt = [fnstring pathExtension];
    NSString *fnFile = [fnstring stringByDeletingPathExtension];

    NSString *textureFile = [[NSBundle mainBundle] pathForResource: fnFile ofType:fnExt ];

//    NSLog( @"pfLoadTexture:   file '%@' ext '%@' will load %@", fnFile, fnExt, textureFile );

    glGetError();
    
    NSError *error = nil;
    GLKTextureInfo *texture = [GLKTextureLoader textureWithContentsOfFile: textureFile
                                                                  options: @{ GLKTextureLoaderOriginBottomLeft : @(YES),
                                                                              GLKTextureLoaderGenerateMipmaps  : @(YES) }
                                                                    error:&error ];
    if (error) {
        NSLog( @"Error loading texture '%s': %@, %@", filename, error, GLKTextureLoaderErrorKey );
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, texture.name );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

    return texture.name;
}


std::string pfPathToResource( const char *resource )
{
    NSString *fnstring = [NSString stringWithUTF8String:resource ];
    NSString *fnExt = [fnstring pathExtension];
    NSString *fnFile = [fnstring stringByDeletingPathExtension];
    
    NSString *resourceFile = [[NSBundle mainBundle] pathForResource: fnFile ofType:fnExt ];
    
    if (!resourceFile)
    {
        DBG::error( "Could not find resource: %s\n", resource );
        return std::string();
    }
    
    return std::string( [resourceFile UTF8String]);
}

std::string pfGetResourcePath()
{
    std::string resourcePath;
    CFBundleRef mainBundle = CFBundleGetMainBundle();
//    CFURLRef bundleURL = CFBundleCopyBundleURL(mainBundle);
//	CFStringRef str = CFURLCopyFileSystemPath( bundleURL, kCFURLPOSIXPathStyle );
//	CFRelease(bundleURL);
	char path[PATH_MAX];
//	
//	CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
//	CFRelease(str);
//    
//    resourcePath = path;
    
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	CFStringRef str = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle );
	CFRelease(resourcesURL);
    
    CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
	CFRelease(str);
    
//    resourcePath = resourcePath + "/" + std::string(path);
    resourcePath = path;
    
    return resourcePath;
}