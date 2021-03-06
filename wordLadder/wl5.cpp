#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <set>
#include <list>
#include <map>
#include <climits>
#include <algorithm>
#include <time.h>

using namespace std;
static bool pflag = true;

#define printf if(pflag)printf

unsigned long curTimeMs()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000*1000+t.tv_nsec/1000;
}
void printVecVec(vector<vector<string> >& vecvec)
{
    bool tmp = pflag;
    pflag=true;
    for (int i = 0; i < vecvec.size(); i++) {
        printf(" vec[%d]= ", i);

        for (int j = 0; j < vecvec[i].size(); j++) {
            printf(" %s ", vecvec[i][j].c_str());
        }

        printf("\n");
    }
    pflag=tmp;
}
void printVec(vector<string> & vec)
{
    printf(" vec= ");

    for (int j = 0; j < vec.size(); j++) {
        printf(" %s ", vec[j].c_str());
    }

    printf("\n");
}

class Solution
{
public:
    vector<vector<string> > findLadders(string beginWord, string endWord, vector<string>& wordList) {
        minDepth = INT_MAX;
        tnode * beginNode = new tnode(beginWord, 0);
        {
            unsigned long curTime1 = curTimeMs();
            createNodes(wordList);
            createGraph(beginNode, wordList, endWord, 0);
            unsigned long curTime2 = curTimeMs();
            bool tmp=pflag;
            pflag=true;
            printf("======create Graph use %lu micro seconds\n", curTime2-curTime1);
            pflag=tmp;
        }
        {
            unsigned long curTime1 = curTimeMs();
            updateDepth(beginNode);
            unsigned long curTime2 = curTimeMs();
            bool tmp=pflag;
            pflag=true;
            printf("======update depth use %lu micro seconds\n", curTime2-curTime1);
            pflag=tmp;
        }
        {
            unsigned long curTime1 = curTimeMs();
            depGraph(beginNode, wordList, beginWord, endWord, 0);
            unsigned long curTime2 = curTimeMs();
            bool tmp=pflag;
            pflag=true;
            printf("======find solution use %lu micro seconds\n", curTime2-curTime1);
            pflag=tmp;
        }
        return ansVec;
    }
private:
    bool isCon(string beginWord, string word) {
        int diff = 0;

        if (beginWord.size() != word.size()) return false;

        for (int i = 0; i < beginWord.size(); i++) {
            if (beginWord[i] != word[i]) diff++;

            if (diff > 1) return false;//more than one diff
        }

        if (diff == 1)//must be one diff
            return true;
        else //same word
            return false;
    }

    int minDepth;
    vector<string> curAns;
    vector<vector<string> > ansVec;

//c style solution
    class tnode
    {
    public:
        string value;
        int depth;
        map<string, tnode *> to_cons;
        map<string, tnode *> from_cons;
        bool init;
        bool updated;
        tnode(string v, int d)
        {
            value=v;
            depth=d;
            init=false;
            updated=false;
        }
    };
    map<string, tnode*> nodeMap;

    void createNodes(vector<string>& wordList)
    {
        for (int i = 0; i < wordList.size(); i++) {
            tnode *p = new tnode(wordList[i], INT_MAX);
            nodeMap.insert(pair<string, tnode*>(wordList[i], p));
        }
    }
    void createGraph(tnode * curNode, vector<string>& wordList, string endWord, int d) {
        if (curNode==NULL) return;
        string key=curNode->value;
        if (key == endWord) return;

        map<string, tnode * >::iterator itr = nodeMap.find(key);
        if (itr!=nodeMap.end() && itr->second->init==true) {
            return;
        }

        printf("---creating %s[%p] start---\n", key.c_str(), curNode);
        for (int i = 0; i < wordList.size(); i++) {
            if (isCon(key, wordList[i])) {
                //map<string,tnode *>::iterator to_itr = curNode->to_cons.find(wordList[i]);
                map<string,tnode *>::iterator to_itr = nodeMap.find(wordList[i]);
                if (to_itr==nodeMap.end()) { //not in connection set
                    tnode *p = new tnode(wordList[i], curNode->depth+1);
                    nodeMap.insert(pair<string, tnode*>(wordList[i], p));
                    curNode->to_cons.insert(pair<string, tnode *>(wordList[i], p));
                    printf("[NULL]=== insert %s(%d)[%p] --> %s(%d)[%p]---\n", key.c_str(), curNode->depth, curNode, wordList[i].c_str(), curNode->depth+1, p); 
                }
                else {
                    tnode *to = to_itr->second;
                    if (to->depth>curNode->depth+1) {
                        //printf("[INORDER]===add  %s(%d)[%p] --> %s(%d)[%p] \n", key.c_str(), curNode->depth, curNode, wordList[i].c_str(), to->depth, to);
                        printf("[createGraph]: update to node %s 's depth from %d to %d\n", to->value.c_str(), to->depth, curNode->depth+1);
                        to->depth=curNode->depth+1;
                        to->updated=true;
                        updateDepth(to);
                        //curNode->to_cons.insert(pair<string, tnode *>(wordList[i], to));
                    }
                    else if (to->depth<curNode->depth){
                        if (curNode->depth>to->depth+1) {
                            printf("[createGraph]: update curnode %s 's depth from %d to %d\n", curNode->value.c_str(), curNode->depth, to->depth+1);
                            curNode->depth=to->depth+1;
                            //curNode->updated=true;
                            updateDepth(curNode);
                        }
                        //map<string, tnode *>::iterator totoitr=to->to_cons.find(key);
                        //if (totoitr==to->to_cons.end()) {
                            //printf("[REVERSE]===rev add %s(%d)[%p] --> %s(%d)[%p] \n", wordList[i].c_str(), to->depth,to, key.c_str(), curNode->depth, curNode);
                            //to->to_cons.insert(pair<string, tnode *>(key, curNode));
                        //}
                    }
                    //else {
                        //printf("=== %s(%d)[%p] and %s(%d)[%p] has same depth\n", key.c_str(), curNode->depth, curNode, wordList[i].c_str(), to->depth, to);
                        //printf("[SAME]===add  %s(%d)[%p] --> %s(%d)[%p] \n", key.c_str(), curNode->depth, curNode, wordList[i].c_str(), to->depth, to);
                        map<string, tnode *>::iterator curitr=curNode->to_cons.find(wordList[i]);
                        if (curitr==curNode->to_cons.end()) {
                            printf("===add  %s(%d)[%p] --> %s(%d)[%p] \n", key.c_str(), curNode->depth, curNode, wordList[i].c_str(), to->depth, to);
                            curNode->to_cons.insert(pair<string, tnode *>(wordList[i], to));
                        }
                        map<string, tnode *>::iterator totoitr=to->to_cons.find(key);
                        if (totoitr==to->to_cons.end()) {
                            printf("===add %s(%d)[%p] --> %s(%d)[%p] \n", wordList[i].c_str(), to->depth,to, key.c_str(), curNode->depth, curNode);
                            to->to_cons.insert(pair<string, tnode *>(key, curNode));
                        }
                            
                    //}
                }
            }
        }
        if (itr==nodeMap.end()){
            curNode->init=true;
            nodeMap.insert(pair<string, tnode*>(key, curNode));
        }
        else {
            itr->second->init=true;
        }
        map<string, tnode*>::iterator it = curNode->to_cons.begin();
        while (it != curNode->to_cons.end()) {
            createGraph(it->second, wordList, endWord, curNode->depth+1);
            it++;
        }

        printf("---creating %s(%d)[%p] cons=%lu finish---\n", curNode->value.c_str(), curNode->depth, curNode,curNode->to_cons.size());
    }

    void updateDepth(tnode * curNode)
    {
        if (curNode==NULL) return;
        map<string, tnode*>::iterator it = curNode->to_cons.begin();
        while (it != curNode->to_cons.end()) {
            tnode * to = it->second;
            if (to->depth >curNode->depth+1) {
                //printf("[updateDepth]: update to node %s [%d]'s depth from %d to %d\n", to->value.c_str(), to->updated, to->depth, curNode->depth+1);
                to->depth=curNode->depth+1;
                updateDepth(to);
            }
            it++;
        }

    }
    void depGraph(tnode * curNode, vector<string>& wordList, string beginWord, string endWord, int d) {
        if (curNode == NULL) return;
        string key = curNode->value;
        if (key == endWord) return;
        //map<string, tnode * >::iterator nitr = nodeMap.find(key);
        //if (nitr == nodeMap.end()) {
        //printf("wrong Node: %s\n", key.c_str());
        //return;
        //}

        //tnode * curNode=nitr->second;
        printf("---dep %s(%d)[%p] start, cons=%lu---\n", key.c_str(), curNode->depth, curNode,curNode->to_cons.size());
        if (curNode->depth>minDepth) return;

        bool updated=false;
        //depth++;
        curAns.push_back(key);
        if (curNode->depth > d) {
            printf(" %s[%p] is updated from %d to %d depth\n",  key.c_str(),curNode, curNode->depth, d);
            curNode->depth=d;
            updated=true;
        }
        //if (updated==false) {
        //depth--;
        //curAns.pop_back();
        //return;
        //}
        map<string, tnode *>::iterator cons_itr=curNode->to_cons.begin();
        while(cons_itr!=curNode->to_cons.end()) {
            tnode * nodeP = cons_itr->second;
            string conStr = nodeP->value;
            printf("---dep %s(%d)[%p] --> %s(%d)[%p]---\n", key.c_str(), curNode->depth,curNode, conStr.c_str(), nodeP->depth, nodeP);
            //updateCount(key, 2);
            if (conStr == endWord) {
                int nodeMinDep = curNode->depth+1;//citr1->second.size();

                if (nodeMinDep < minDepth) {
                    printf("find one better solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                    minDepth = nodeMinDep;
                    curAns.push_back(conStr);
                    printVec(curAns);
                    ansVec.push_back(curAns);
                    curAns.pop_back();
                } else if (nodeMinDep == minDepth) {
                    printf("find one more solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                    curAns.push_back(conStr);
                    printVec(curAns);
                    ansVec.push_back(curAns);
                    curAns.pop_back();
                }
                else {
                    printf("find one worse solution cur=%d > min=%d\n", nodeMinDep, minDepth);
                    printVec(curAns);
                }
            }
            cons_itr++;
        }

        //if (updated==true) {
        //allNodes.erase(k);
        //}
        //allNodes.insert(pair<string, set<string> >(k, cons));
        //set<string>::iterator it = cons.begin();
        map<string, tnode *>::iterator node_itr=curNode->to_cons.begin();
        //cons_itr=curNode->to_cons.begin();
        while (node_itr != curNode->to_cons.end()) {
            //map<string, tnode *>::iterator node_itr = nodeMap.find((*cons_itr1)->value);
            //if (node_itr!=nodeMap.end()) {
            tnode *p=node_itr->second;
            if (p->depth < d+1) {
                printf("[depGraph]don't go back fromn %s(%d) --> %s(%d)\n", key.c_str(), d+1, p->value.c_str(), p->depth);
                //p->depth=d+1;
            }
            else
                depGraph(p, wordList, beginWord, endWord, d+1);
            //}
            node_itr++;
        }

        curAns.pop_back();
        //depth--;
        printf("---dep %s [%d] finish---\n", key.c_str(), d);
    }

};



int main(int argc, char * argv[])
{
    string start1="qa";
    string end1="sq";
    string p1[] = {"si","go","se","cm","so","ph","mt","db","mb","sb","kr","ln","tm","le","av","sm","ar","ci","ca","br","ti","ba","to","ra","fa","yo","ow","sn","ya","cr","po","fe","ho","ma","re","or","rn","au","ur","rh","sr","tc","lt","lo","as","fr","nb","yb","if","pb","ge","th","pm","rb","sh","co","ga","li","ha","hz","no","bi","di","hi","qa","pi","os","uh","wm","an","me","mo","na","la","st","er","sc","ne","mn","mi","am","ex","pt","io","be","fm","ta","tb","ni","mr","pa","he","lr","sq","ye"};
    string start2 = "hit";
    string end2 = "cog";
    string p2[] = {"hot", "dot", "dog", "lot", "log", "cog"};
    string start3 = "cet";
    string end3 = "ism";
    string p3[] = {"kid", "tag", "pup", "ail", "tun", "woo", "erg", "luz", "brr", "gay", "sip", "kay", "per", "val", "mes", "ohs", "now", "boa", "cet", "pal", "bar", "die", "war", "hay", "eco", "pub", "lob", "rue", "fry", "lit", "rex", "jan", "cot", "bid", "ali", "pay", "col", "gum", "ger", "row", "won", "dan", "rum", "fad", "tut", "sag", "yip", "sui", "ark", "has", "zip", "fez", "own", "ump", "dis", "ads", "max", "jaw", "out", "btu", "ana", "gap", "cry", "led", "abe", "box", "ore", "pig", "fie", "toy", "fat", "cal", "lie", "noh", "sew", "ono", "tam", "flu", "mgm", "ply", "awe", "pry", "tit", "tie", "yet", "too", "tax", "jim", "san", "pan", "map", "ski", "ova", "wed", "non", "wac", "nut", "why", "bye", "lye", "oct", "old", "fin", "feb", "chi", "sap", "owl", "log", "tod", "dot", "bow", "fob", "for", "joe", "ivy", "fan", "age", "fax", "hip", "jib", "mel", "hus", "sob", "ifs", "tab", "ara", "dab", "jag", "jar", "arm", "lot", "tom", "sax", "tex", "yum", "pei", "wen", "wry", "ire", "irk", "far", "mew", "wit", "doe", "gas", "rte", "ian", "pot", "ask", "wag", "hag", "amy", "nag", "ron", "soy", "gin", "don", "tug", "fay", "vic", "boo", "nam", "ave", "buy", "sop", "but", "orb", "fen", "paw", "his", "sub", "bob", "yea", "oft", "inn", "rod", "yam", "pew", "web", "hod", "hun", "gyp", "wei", "wis", "rob", "gad", "pie", "mon", "dog", "bib", "rub", "ere", "dig", "era", "cat", "fox", "bee", "mod", "day", "apr", "vie", "nev", "jam", "pam", "new", "aye", "ani", "and", "ibm", "yap", "can", "pyx", "tar", "kin", "fog", "hum", "pip", "cup", "dye", "lyx", "jog", "nun", "par", "wan", "fey", "bus", "oak", "bad", "ats", "set", "qom", "vat", "eat", "pus", "rev", "axe", "ion", "six", "ila", "lao", "mom", "mas", "pro", "few", "opt", "poe", "art", "ash", "oar", "cap", "lop", "may", "shy", "rid", "bat", "sum", "rim", "fee", "bmw", "sky", "maj", "hue", "thy", "ava", "rap", "den", "fla", "auk", "cox", "ibo", "hey", "saw", "vim", "sec", "ltd", "you", "its", "tat", "dew", "eva", "tog", "ram", "let", "see", "zit", "maw", "nix", "ate", "gig", "rep", "owe", "ind", "hog", "eve", "sam", "zoo", "any", "dow", "cod", "bed", "vet", "ham", "sis", "hex", "via", "fir", "nod", "mao", "aug", "mum", "hoe", "bah", "hal", "keg", "hew", "zed", "tow", "gog", "ass", "dem", "who", "bet", "gos", "son", "ear", "spy", "kit", "boy", "due", "sen", "oaf", "mix", "hep", "fur", "ada", "bin", "nil", "mia", "ewe", "hit", "fix", "sad", "rib", "eye", "hop", "haw", "wax", "mid", "tad", "ken", "wad", "rye", "pap", "bog", "gut", "ito", "woe", "our", "ado", "sin", "mad", "ray", "hon", "roy", "dip", "hen", "iva", "lug", "asp", "hui", "yak", "bay", "poi", "yep", "bun", "try", "lad", "elm", "nat", "wyo", "gym", "dug", "toe", "dee", "wig", "sly", "rip", "geo", "cog", "pas", "zen", "odd", "nan", "lay", "pod", "fit", "hem", "joy", "bum", "rio", "yon", "dec", "leg", "put", "sue", "dim", "pet", "yaw", "nub", "bit", "bur", "sid", "sun", "oil", "red", "doc", "moe", "caw", "eel", "dix", "cub", "end", "gem", "off", "yew", "hug", "pop", "tub", "sgt", "lid", "pun", "ton", "sol", "din", "yup", "jab", "pea", "bug", "gag", "mil", "jig", "hub", "low", "did", "tin", "get", "gte", "sox", "lei", "mig", "fig", "lon", "use", "ban", "flo", "nov", "jut", "bag", "mir", "sty", "lap", "two", "ins", "con", "ant", "net", "tux", "ode", "stu", "mug", "cad", "nap", "gun", "fop", "tot", "sow", "sal", "sic", "ted", "wot", "del", "imp", "cob", "way", "ann", "tan", "mci", "job", "wet", "ism", "err", "him", "all", "pad", "hah", "hie", "aim", "ike", "jed", "ego", "mac", "baa", "min", "com", "ill", "was", "cab", "ago", "ina", "big", "ilk", "gal", "tap", "duh", "ola", "ran", "lab", "top", "gob", "hot", "ora", "tia", "kip", "han", "met", "hut", "she", "sac", "fed", "goo", "tee", "ell", "not", "act", "gil", "rut", "ala", "ape", "rig", "cid", "god", "duo", "lin", "aid", "gel", "awl", "lag", "elf", "liz", "ref", "aha", "fib", "oho", "tho", "her", "nor", "ace", "adz", "fun", "ned", "coo", "win", "tao", "coy", "van", "man", "pit", "guy", "foe", "hid", "mai", "sup", "jay", "hob", "mow", "jot", "are", "pol", "arc", "lax", "aft", "alb", "len", "air", "pug", "pox", "vow", "got", "meg", "zoe", "amp", "ale", "bud", "gee", "pin", "dun", "pat", "ten", "mob"};
    string start4="red";
    string end4="tax";
    string p4[]= {"ted","tex","red","tax","tad","den","rex","pee"};
    string start5="red";
    string end5="tax";
    string p5[]= {"ted","tex","tax","tad","rex","tix", "tkx"};
    vector<string> input1(std::begin(p1), std::end(p1));
    vector<string> input2(std::begin(p2), std::end(p2));
    vector<string> input3(std::begin(p3), std::end(p3));
    vector<string> input4(std::begin(p4), std::end(p4));
    vector<string> input5(std::begin(p5), std::end(p5));
    //vector<string> input;
    //copy(std::begin(p), std::end(p), input);
    Solution so;
    int choose = 1;
    if (argc>=2) {
        choose = atoi(argv[1]);
    }
    if (argc>=3) {
        pflag = atoi(argv[2]);
    }
    vector<vector<string> > ans;
    switch(choose) {
    case 1:
        printf("input has %lu elements\n", input1.size());
        ans = so.findLadders(start1, end1, input1);
        break;
    case 2:
        printf("input has %lu elements\n", input2.size());
        ans = so.findLadders(start2, end2, input2);
        break;
    case 3:
        printf("input has %lu elements\n", input3.size());
        ans = so.findLadders(start3, end3, input3);
        break;
    case 4:
        printf("input has %lu elements\n", input4.size());
        ans = so.findLadders(start4, end4, input4);
        break;
    case 5:
        printf("input has %lu elements\n", input5.size());
        ans = so.findLadders(start5, end5, input5);
        break;
    }

    printf("=======done======\n");
    printVecVec(ans);
}

