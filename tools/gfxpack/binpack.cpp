#include <vector>
#include <algorithm>

#include "binpack.h"
#include "image.h"
#include "chip.h"

struct PackNode
{
    PackNode() : left(NULL), right(NULL), chip(NULL) {}
    ~PackNode()
    {
        delete left;
        delete right;        
    }
    
    PackNode *left, *right;
    int w, h, x, y; // rect
    Chip *chip;
};

bool _chipMaxDimLT(  Chip *a,  Chip *b ) {

	int am = std::max( a->m_width, a->m_height ),
        bm = std::max( b->m_width, b->m_height );

	return am < bm;
}

PackNode *insertNode( Chip *c, PackNode *curr )
{
    PackNode *newNode;
    
    if ((curr->left) || (curr->right))
    {
        // not a leaf, try left 
        newNode = insertNode( c, curr->left );
        if (!newNode)
        {
            // didn't fit, try right
            return insertNode( c, curr->right );            
        }
        else
        {
            return newNode;            
        }        
    }
    else
    {
        // It's a leaf node
        if (curr->chip)
        {
            // already used
            return NULL;            
        }
        
        // will this chip fit?
        if ((c->m_width > curr->w) ||
            (c->m_height > curr->h))
        {
            // doesn't fit
            return NULL;            
        }
        
        // does it fit exactly?
        if ((c->m_width == curr->w) &&
            (c->m_height == curr->h))
        {
            curr->chip = c; // yay
            c->m_xpos = curr->x;
            c->m_ypos = curr->y;
            
            return curr;            
        }

        // ok, chop it up
        curr->left = new PackNode();
        curr->right = new PackNode();
        
        int dw = curr->w - c->m_width;
        int dh = curr->h - c->m_height;
        
        if (dw > dh)
        {
            // chop horizontally
            curr->left->x = curr->x;
            curr->left->y = curr->y;
            curr->left->w = c->m_width;
            curr->left->h = curr->h;
            
            curr->right->x = curr->x + c->m_width;
            curr->right->y = curr->y;
            curr->right->w = curr->w - c->m_width;
            curr->right->h = curr->h;            
        }
        else
        {
            // chop vertically
            curr->left->x = curr->x;
            curr->left->y = curr->y;
            curr->left->w = curr->w;
            curr->left->h = c->m_height;
            
            curr->right->x = curr->x;
            curr->right->y = curr->y + c->m_height;
            curr->right->w = curr->w;
            curr->right->h = curr->h - c->m_height;
        }

        // put chip in left child node
        // which should be exactly the right size
        return insertNode( c, curr->left );        
    }
    
    // shouldn't be reachable unless space is filled up
    return NULL;    
}

FpImage *drawChips( std::vector<Chip*> &chips, int pack_w, int pack_h,
                    unsigned long bgColor)
{
    FpImage *img = new FpImage( pack_w, pack_h, bgColor );
    
    for (std::vector<Chip*>::iterator ci = chips.begin();
         ci != chips.end(); ++ci )
    {
        img->paste( *(*ci)->m_img, (*ci)->m_xpos, (*ci)->m_ypos );        
    }

    return img;    
    
}


void initializePack( std::vector<Chip*> &chips, std::vector<Chip*> &chipsLeft )
{
    // clear the list of stuff to pack
    chipsLeft.erase( chipsLeft.begin(), chipsLeft.end() );
    
    // copy the list into there
    std::copy( chips.begin(), chips.end(), std::back_inserter( chipsLeft) );

    // Sort by max dimension
    std::sort( chipsLeft.begin(), chipsLeft.end(), _chipMaxDimLT );    
    
}


FpImage *packChips( std::vector<Chip*> chips, unsigned long bgColor )
{
    std::vector<Chip*> chipsLeft;
    
    PackNode *root = NULL;

    bool done = false;

    int pack_w, pack_h;

    // Make sure there's something to do
    if (!chips.size())
    {
        return NULL;        
    }
    
    
    // just start with the size of the first glyph
    int maxDim = std::max( chips[0]->m_width, chips[0]->m_height );    
    pack_w = maxDim;    
    pack_h = maxDim;
    
    
    while (!done)
    {        
        // Need to start or restart the tree?
        if  (!root)
        {
            initializePack( chips, chipsLeft );
            
            root = new PackNode();
            root->w = pack_w;
            root->h = pack_h;
            root->x = 0;
            root->y = 0;
        }

        // Is there anything left to pack?
        if (!chipsLeft.size())
        {
            // delete tree, positions are already copied into chips
            delete root;            
            return drawChips( chips, pack_w, pack_h, bgColor );
            
        }
        
        // Get the next chip to pack
        Chip *c = chipsLeft.back();
        
        if (!insertNode( c, root ))
        {
            // can't make it fit, grow the space and start over
            pack_w += 1;
            pack_h += 1;

            // reset the layout
            delete root;            
            root = NULL;            
        }
        else
        {
            // pop the chip we just packed
            chipsLeft.pop_back();            
        }
        
        
        
    }
    
    
}
