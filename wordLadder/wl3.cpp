#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <set>
#include <list>
#include <map>
#include <climits>
#include <algorithm>

using namespace std;

void printVecVec(vector<vector<string> >& vecvec)
{
    for (int i = 0; i < vecvec.size(); i++) {
        printf(" vec[%d]= ", i);

        for (int j = 0; j < vecvec[i].size(); j++) {
            printf(" %s ", vecvec[i][j].c_str());
        }

        printf("\n");
    }
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
        createGraph(beginWord, wordList, beginWord, endWord, 1);
        printf("======create graph done mindepth=%d ===========\n", minDepth);
        printCount();
        ansVec.clear();
        curAns.clear();
        allNodes.clear();
        curDepth = 0;
        depth = 0;
        printf("=======start search==========\n");
        searchGraph(beginWord, wordList, endWord, 1);
        printCount();
        //findMinPath(beginWord, endWord);
        return ansVec;
    }
    Solution() {
        minDepth = INT_MAX;
        curDepth = 0;
        depth = 0;
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

    void printCount()
    {
        printf("Count = ");
        map<string, int>::iterator itr = count.begin();
        while(itr!=count.end()) {
            printf(" %s(%d) ", itr->first.c_str(), itr->second);
            itr++;
        }
        printf("\n");
    }
    void updateCount(string k, int v)
    {
        map<string, int>::iterator itr = count.find(k);
        if (itr==count.end()) {
            count.insert(pair<string, int>(k, v));
        }
        else {
            int value=itr->second;
            count.erase(k);
            count.insert(pair<string, int>(k, value+v));
        }
    }

    map<string, set<string> > allNodes;
    int depth;
    map<string, int> count;
    map<string, int>  c;
    int curDepth;
    int minDepth;
    vector<string> curAns;
    vector<vector<string> > ansVec;

    void createGraph(string k, vector<string> wordList, string beginWord, string endWord, int d) {
        if (k == endWord) return;
        //if (k == beginWord) return;

        map<string, set<string> >::iterator itr = allNodes.find(k);
        set<string> cons;
        //printf("---creating %s start---\n", k.c_str());
        //if (count.size()<wordList.size())
        bool updated=false;
        depth++;
        curAns.push_back(k);
        if (itr != allNodes.end()) {
            depth--;
            curAns.pop_back();
            return;
        }
        //printVec(curAns);
        for (int i = 0; i < wordList.size(); i++) {
            if (isCon(k, wordList[i])) {
                updateCount(k, 2);
                if (cons.find(wordList[i]) == cons.end()) {
                    cons.insert(wordList[i]);
                    if (wordList[i] == endWord) {
                        int nodeMinDep = d+1;//citr1->second.size();

                        if (nodeMinDep < minDepth) {
                            printf("find one better solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                            minDepth = nodeMinDep;
                            curAns.push_back(wordList[i]);
                            printVec(curAns);
                            ansVec.push_back(curAns);
                            curAns.pop_back();
                        } else if (nodeMinDep == minDepth) {
                            printf("find one more solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                            curAns.push_back(wordList[i]);
                            printVec(curAns);
                            ansVec.push_back(curAns);
                            curAns.pop_back();
                        }
                        else {
                            //printf("find one worse solution cur=%d > min=%d\n", nodeMinDep, minDepth);
                            //printVec(citr1->second);
                        }
                    }

                    //printf("======= creating %s --> %s, depth=%d ====\n", k.c_str(), wordList[i].c_str(), depth);
                }
            }
        }

        //if (updated==true) {
            //allNodes.erase(k);
        //}
        allNodes.insert(pair<string, set<string> >(k, cons));
        set<string>::iterator it = cons.begin();

        while (it != cons.end()) {
            createGraph(*it, wordList, beginWord, endWord, d+1);
            it++;
        }

        curAns.pop_back();
        depth--;
        //printf("---creating %s finish---\n", k.c_str());
    }


    void searchGraph(string k, vector<string> wordList, string endWord, int d) {
        if (k == endWord) return;
        if (d+1>minDepth) return;

        map<string, set<string> >::iterator itr = allNodes.find(k);
        set<string> cons;
        printf("---searching %s start---\n", k.c_str());
        bool updated=false;
        depth++;
        curAns.push_back(k);
        {
            map<string, int>::iterator citr = c.find(k);
            if (citr==c.end()) {
                c.insert(pair<string, int>(k, d+1));
                //updated=true;
                printf(" %s is inserted with %d depth\n",  k.c_str(), d+1);
            }
            else {
                if (citr->second>d+1) {
                    printf(" %s is updated from %d to %d depth\n",  k.c_str(), citr->second, d+1);
                    c.erase(k);
                    c.insert(pair<string, int>(k, d+1));
                    updated=true;
                }
            }
        }
        {
            map<string, int>::iterator citr = count.find(k);
            if (citr!=count.end()) {
                if (citr->second>0) updated=true;
            }
        }

        if (itr != allNodes.end() && updated==false) {
            printf(" %s has been processed and no update, skip\n", k.c_str());
            depth--;
            curAns.pop_back();
            return;
        }
        //printVec(curAns);
        for (int i = 0; i < wordList.size(); i++) {
            if (isCon(k, wordList[i])) {
            updateCount(k, -1);
                if (cons.find(wordList[i]) == cons.end()) {
                    printf("======= trying %s --> %s, depth=%d ====\n", k.c_str(), wordList[i].c_str(), d+1);
                    cons.insert(wordList[i]);
                    //map<string, int>::iterator citr = c.find(k);
                    
                    if (wordList[i] == endWord) {
                        //map<string, vector<string> >::iterator citr1 = count.find(wordList[i]);
                        int nodeMinDep = d+1;//citr1->second.size();

                        if (nodeMinDep < minDepth) {
                            printf("find one better solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                            ansVec.clear();
                            minDepth = nodeMinDep;
                            curAns.push_back(wordList[i]);
                            printVec(curAns);
                            ansVec.push_back(curAns);
                            for( int j=0;j<curAns.size()-1;j++) {
                                map<string, set<string> >::iterator itr = allNodes.find(curAns[j]);
                                if (itr==allNodes.end()) continue;
                                set<string> tempCons = itr->second;
                                allNodes.erase(itr);
                                tempCons.erase(curAns[j+1]);
                                allNodes.insert(pair<string, set<string> >(curAns[j], tempCons));
                            }
                            curAns.pop_back();
                        } else if (nodeMinDep == minDepth) {
                            printf("find one more solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                            curAns.push_back(wordList[i]);
                            printVec(curAns);
                            ansVec.push_back(curAns);
                            for( int j=0;j<curAns.size()-1;j++) {
                                map<string, set<string> >::iterator itr = allNodes.find(curAns[j]);
                                if (itr==allNodes.end()) continue;
                                set<string> tempCons = itr->second;
                                allNodes.erase(itr);
                                tempCons.erase(curAns[j+1]);
                                allNodes.insert(pair<string, set<string> >(curAns[j], tempCons));
                            }
                            curAns.pop_back();
                        }
                        else {
                            printf("[search]find one worse solution cur=%d > min=%d\n", nodeMinDep, minDepth);
                            printVec(curAns);
                        }
                    }

                }
            }
        }

        if (updated==true) {
            allNodes.erase(k);
        }
        allNodes.insert(pair<string, set<string> >(k, cons));
        set<string>::iterator it = cons.begin();

        //map<string, int>::iterator citr = c.find(k);
        while (it != cons.end()) {
            {
               map<string, int>::iterator citr = c.find(*it);
               if (citr!=c.end()) {
                   if (citr->second < d+1 ) {
                       printf("don't go back fromn %s(%d) --> %s(%d)\n", k.c_str(), d+1, (*it).c_str(), citr->second);
                        it++;
                       continue;
                   }
               }
            }
            searchGraph(*it, wordList, endWord, d+1);
            it++;
        }

        curAns.pop_back();
        depth--;
        printf("---searching %s finish---\n", k.c_str());
    }
    void findMinPath(string beginWord, string endWord)
    {
        if (curDepth>=minDepth) {
            return;
        }
        //printf("--processing %s start, curDep=%d\n", beginWord.c_str(), curDepth);
        map<string, set<string> >::iterator itr = allNodes.find(beginWord);
        if (itr == allNodes.end()) {
            return;
        }
        if (std::find(curAns.begin(), curAns.end(), beginWord)!=curAns.end()) {
            //printf("--processing %s is already in curAns, just return\n", beginWord.c_str());
            return;
        }
        //map<string, vector<string> >::iterator citr = count.find(beginWord);
        //if (citr->second.size() < curDepth) {
            //printf("--processing %s 's depth %d > %d, just return\n", beginWord.c_str(), curDepth, citr->second);
            //return;
        //}

        curDepth++;
        curAns.push_back(beginWord);
        set<string> cons = itr->second;
        allNodes.erase(beginWord);

        bool found=false;
        set<string>::iterator it=cons.begin();
        while(it!=cons.end()) {
            //printf("--processing %s's con %s, curDep=%d\n", beginWord.c_str(), (*it).c_str(), curDepth);
            if ((*it)==endWord) {//found it
                if (curDepth==minDepth) {
                    //printf("find one more solution cur=%d == min=%d\n", curDepth, minDepth);
                    curAns.push_back(*it);
                    ansVec.push_back(curAns);
                    curAns.pop_back();
                    //printVec(curAns);
                    //printVecVec(ansVec);
                    found=true;
                    break;
                }
                else {
                    //printf("find one better solution cur=%d < min=%d\n", curDepth, minDepth);
                    minDepth=curDepth;
                    ansVec.clear();
                    curAns.push_back(*it);
                    ansVec.push_back(curAns);
                    curAns.pop_back();
                    //printVec(curAns);
                    //printVecVec(ansVec);
                    found=true;
                    break;
                }
            }
            it++;
        }

        if (found==false) {
            set<string>::iterator it=cons.begin();
            while(it!=cons.end()) {
                findMinPath(*it, endWord);
                it++;
            }
        }
        curDepth--;
        //cons.clear();
        //if (curDepth==citr->second) allNodes.erase(beginWord);
        //printf("--processing %s end, pop %s, curDep=%d, cons.size=%lu\n", beginWord.c_str(), curAns.back().c_str(), curDepth, cons.size());
        curAns.pop_back();
        allNodes.insert(pair<string, set<string> >(beginWord, cons));
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
    vector<string> input1(std::begin(p1), std::end(p1));
    vector<string> input2(std::begin(p2), std::end(p2));
    vector<string> input3(std::begin(p3), std::end(p3));
    vector<string> input4(std::begin(p4), std::end(p4));
    //vector<string> input;
    //copy(std::begin(p), std::end(p), input);
    Solution so;
    int choose = 1;
    if (argc>=2) {
        choose = atoi(argv[1]);
    }
    vector<vector<string> > ans;
    switch(choose){
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
    }

    printf("=======done======\n");
    printVecVec(ans);
}

