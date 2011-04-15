
import os, sys, string

if len(sys.argv) < 3:
    print "Usage: finfo2cpp file.finfo pixelres"
    sys.exit(1)
    
infile = sys.argv[1]
pixelres = int(sys.argv[2])
outfilename = string.replace( os.path.basename( infile ), '.finfo', '.cpp')
outfile = open( outfilename, 'w' )

fontname = string.capitalize(os.path.splitext( os.path.basename( infile))[0])

# warning message
print """
WARNING: Don't use this. gfxpack can generate a similar cpp file
directly.  This file is included only as an example of how you might
convert a finfo file (the simplist format) to some other custom
format.
"""

#TMP hack
#fontname = "Bookman"

fontDb = {}

# write header
outfile.write("""
#include "font.h"

""");

# Read in finfo file
for line in open(infile).readlines():
    line = line.strip()

    if not len(line):
        continue

    if line[0]=='#':
        continue
        
    ch, pxlsize, baseline, x, y, w, h = map( int, line.split() )
    if (pxlsize == 0):
        continue
        
    if not fontDb.has_key( pxlsize ):
        fontDb[pxlsize] = {}

    s0 = (float(x) /pixelres) 
    s1 = (float(y) /pixelres) 
    st_w = (float(w) /pixelres) 
    st_h = (float(h) /pixelres) 
    fontDb[pxlsize][ch] = (baseline, w, h, (s0, s1, st_w, st_h))
    
    
print "writing", outfilename
# write a function for each point size
sizes = fontDb.keys()
sizes.sort()

for sz in sizes:
    
    sz2 = sz
    print ( "writing size %d (%d)" % (sz, sz2) )
    
    outfile.write("""
Font *makeFont_%s_%d( GLuint texId)
{
    Font *font = new Font( texId, %d);
    
""" % (fontname, sz2, sz2) )
    
    chars = fontDb[sz].keys()
    chars.sort()
    
    for ch in chars:
        g = fontDb[sz][ch]
        st = g[3]
        outfile.write("\tfont->addGlyph( %3d,   %2d, %2d, %2d,   %f, %f, %f, %f ); // '%s'\n" %
                       (ch, g[0], g[1], g[2], st[0], st[1], st[0]+st[2], st[1]+st[3], chr(ch) ) )
    outfile.write( "\n\treturn font;\n}\n\n");
    
outfile.close()
    
