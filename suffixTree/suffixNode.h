/* suffixNode.cpp - describes a data structure for a node in the suffix tree.
Written by Connor Wool, Spring 2018 */

#include <iostream>
#include <string>
#include <vector>

#define SN_IS_ROOT 0x01
#define SN_IS_INTERNAL 0x02
#define SN_IS_LEAF 0x04
#define SN_LINK_KNOWN 0x08

class SuffixNode{
    private:
        int flags;
        int id;
        int stringDepth;
        string edgeLabel;
        SuffixNode *parent;
        SuffixNode *suffixLink;
        vector<SuffixNode*> children;

    public:
        SuffixNode(int id);

        void setParent(SuffixNode *p){
            this->parent = p;
        }
        SuffixNode *getParent(){
            return this->parent;
        }

        void setEdgeLabel()

        //track status as root node
        void setAsRoot(){
            this->flags = (this->flags | SN_IS_ROOT);
        }
        bool isRoot(){
            return (this->flags & SN_IS_ROOT);
        }

        //track status as internal node
        void setAsInternal(){
            this->flags = (this->flags | SN_IS_INTERNAL);
        }
        bool isInternal(){
            return (this->flags & SN_IS_INTERNAL);
        }

        //track status as leaf node
        void setAsLeaf(){
            this->flags = (this->flags | SN_IS_LEAF);
        }
        bool isLeaf(){
            return (this->flags & SN_IS_LEAF);
        }

        void setLinkKnown(){
            this->flags = (this->flags | SN_LINK_KNOWN);
        }
        void setLinkUnknown(){
            this->flags = (this->flags & (~SN_LINK_KNOWN));
        }
        bool linkKnown(){
            return (this->flags & SN_LINK_KNOWN);
        }
}