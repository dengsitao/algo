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

        //for (int i=0;i<wordList.size();i++) {
        createGraph(beginWord, wordList, endWord);
        //string temp = wordList[0];
        //wordList.erase(wordList.begin());
        //wordList.push_back(temp);
        //allNodes.clear();
        //}
        printf("======create graph done mindepth=%d ===========\n", minDepth);
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
    void updateGraph(string k) {
        map<string, vector<string> >::iterator citr = count.find(k);

        if (citr == count.end()) {
            count.insert(pair<string, vector<string> >(k, curAns));
            printf("adding %s to count\n", k.c_str());
        } else if (depth < citr->second.size()) {
            count.erase(k);
            count.insert(pair<string, vector<string> >(k, curAns));
            printf("replacing %s's con list size=%d to count\n", k.c_str(), depth);
        }

    }

    map<string, set<string> > allNodes;
    int depth;
    map<string, vector<string> > count;
    int curDepth;
    int minDepth;
    vector<string> curAns;
    vector<vector<string> > ansVec;

    void createGraph(string k, vector<string> wordList, string endWord) {
        if (k == endWord) return;

        map<string, set<string> >::iterator itr = allNodes.find(k);
        set<string> cons;

        if (itr != allNodes.end()) {
            return;
        }

        printf("---creating %s start---\n", k.c_str());
        //if (count.size()<wordList.size())
        depth++;
        curAns.push_back(k);

        //printVec(curAns);
        for (int i = 0; i < wordList.size(); i++) {
            if (isCon(k, wordList[i])) {
                if (cons.find(wordList[i]) == cons.end()) {
                    cons.insert(wordList[i]);
                    map<string, vector<string> >::iterator citr = count.find(wordList[i]);

                    if (citr == count.end()) {
                        count.insert(pair<string, vector<string> >(wordList[i], curAns));
                        printf("adding %s to count\n", wordList[i].c_str());
                    } else if (depth < citr->second.size()) {
                        count.erase(wordList[i]);
                        count.insert(pair<string, vector<string> >(wordList[i], curAns));
                        printf("replacing %s's con list size=%d to count\n", wordList[i].c_str(), depth);
                    }

                    //}
                    if (wordList[i] == endWord) {
                        map<string, vector<string> >::iterator citr1 = count.find(k);
                        int nodeMinDep = citr1->second.size();

                        if (nodeMinDep < minDepth) {
                            printf("find one better solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                            printVec(citr1->second);
                            ansVec.clear();
                            if (citr1->second!=k) {
                                citr1->second.push_back(k);
                                nodeMinDep++;
                            }
                            citr1->second.push_back(wordList[i]);
                            minDepth = nodeMinDep;
                            ansVec.push_back(citr1->second);
                            citr1->second.pop_back();
                            citr1->second.pop_back();
                            //curAns.push_back(wordList[i]);
                            //ansVec.push_back(curAns);
                            //curAns.pop_back();
                        } else if (nodeMinDep == minDepth) {
                            printf("find one more solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                            printVec(citr1->second);
                            citr1->second.push_back(k);
                            citr1->second.push_back(wordList[i]);
                            ansVec.push_back(citr1->second);
                            citr1->second.pop_back();
                            citr1->second.pop_back();
                            //curAns.push_back(wordList[i]);
                            //ansVec.push_back(curAns);
                            //curAns.pop_back();
                        }
                        else {
                            printf("find one worse solution cur=%d == min=%d\n", nodeMinDep, minDepth);
                            printVec(citr1->second);
                        }
                    }

                    printf("======= add %s --> %s, depth=%d ====\n", k.c_str(), wordList[i].c_str(), depth);
                }
            }
        }

        allNodes.insert(pair<string, set<string> >(k, cons));
        set<string>::iterator it = cons.begin();

        while (it != cons.end()) {
            createGraph(*it, wordList, endWord);
            it++;
        }

        curAns.pop_back();
        depth--;
        printf("---creating %s finish---\n", k.c_str());
    }

};



int main(int argc, char * argv[])
{
    //string start="qa";
    //string end="sq";
    //string p[] = {"si","go","se","cm","so","ph","mt","db","mb","sb","kr","ln","tm","le","av","sm","ar","ci","ca","br","ti","ba","to","ra","fa","yo","ow","sn","ya","cr","po","fe","ho","ma","re","or","rn","au","ur","rh","sr","tc","lt","lo","as","fr","nb","yb","if","pb","ge","th","pm","rb","sh","co","ga","li","ha","hz","no","bi","di","hi","qa","pi","os","uh","wm","an","me","mo","na","la","st","er","sc","ne","mn","mi","am","ex","pt","io","be","fm","ta","tb","ni","mr","pa","he","lr","sq","ye"};
    string start = "hit";
    string end = "cog";
    string p[] = {"hot", "dot", "dog", "lot", "log", "cog"};
    //string start = "cet";
    //string end = "ism";
    //string p[] = {"kid", "tag", "pup", "ail", "tun", "woo", "erg", "luz", "brr", "gay", "sip", "kay", "per", "val", "mes", "ohs", "now", "boa", "cet", "pal", "bar", "die", "war", "hay", "eco", "pub", "lob", "rue", "fry", "lit", "rex", "jan", "cot", "bid", "ali", "pay", "col", "gum", "ger", "row", "won", "dan", "rum", "fad", "tut", "sag", "yip", "sui", "ark", "has", "zip", "fez", "own", "ump", "dis", "ads", "max", "jaw", "out", "btu", "ana", "gap", "cry", "led", "abe", "box", "ore", "pig", "fie", "toy", "fat", "cal", "lie", "noh", "sew", "ono", "tam", "flu", "mgm", "ply", "awe", "pry", "tit", "tie", "yet", "too", "tax", "jim", "san", "pan", "map", "ski", "ova", "wed", "non", "wac", "nut", "why", "bye", "lye", "oct", "old", "fin", "feb", "chi", "sap", "owl", "log", "tod", "dot", "bow", "fob", "for", "joe", "ivy", "fan", "age", "fax", "hip", "jib", "mel", "hus", "sob", "ifs", "tab", "ara", "dab", "jag", "jar", "arm", "lot", "tom", "sax", "tex", "yum", "pei", "wen", "wry", "ire", "irk", "far", "mew", "wit", "doe", "gas", "rte", "ian", "pot", "ask", "wag", "hag", "amy", "nag", "ron", "soy", "gin", "don", "tug", "fay", "vic", "boo", "nam", "ave", "buy", "sop", "but", "orb", "fen", "paw", "his", "sub", "bob", "yea", "oft", "inn", "rod", "yam", "pew", "web", "hod", "hun", "gyp", "wei", "wis", "rob", "gad", "pie", "mon", "dog", "bib", "rub", "ere", "dig", "era", "cat", "fox", "bee", "mod", "day", "apr", "vie", "nev", "jam", "pam", "new", "aye", "ani", "and", "ibm", "yap", "can", "pyx", "tar", "kin", "fog", "hum", "pip", "cup", "dye", "lyx", "jog", "nun", "par", "wan", "fey", "bus", "oak", "bad", "ats", "set", "qom", "vat", "eat", "pus", "rev", "axe", "ion", "six", "ila", "lao", "mom", "mas", "pro", "few", "opt", "poe", "art", "ash", "oar", "cap", "lop", "may", "shy", "rid", "bat", "sum", "rim", "fee", "bmw", "sky", "maj", "hue", "thy", "ava", "rap", "den", "fla", "auk", "cox", "ibo", "hey", "saw", "vim", "sec", "ltd", "you", "its", "tat", "dew", "eva", "tog", "ram", "let", "see", "zit", "maw", "nix", "ate", "gig", "rep", "owe", "ind", "hog", "eve", "sam", "zoo", "any", "dow", "cod", "bed", "vet", "ham", "sis", "hex", "via", "fir", "nod", "mao", "aug", "mum", "hoe", "bah", "hal", "keg", "hew", "zed", "tow", "gog", "ass", "dem", "who", "bet", "gos", "son", "ear", "spy", "kit", "boy", "due", "sen", "oaf", "mix", "hep", "fur", "ada", "bin", "nil", "mia", "ewe", "hit", "fix", "sad", "rib", "eye", "hop", "haw", "wax", "mid", "tad", "ken", "wad", "rye", "pap", "bog", "gut", "ito", "woe", "our", "ado", "sin", "mad", "ray", "hon", "roy", "dip", "hen", "iva", "lug", "asp", "hui", "yak", "bay", "poi", "yep", "bun", "try", "lad", "elm", "nat", "wyo", "gym", "dug", "toe", "dee", "wig", "sly", "rip", "geo", "cog", "pas", "zen", "odd", "nan", "lay", "pod", "fit", "hem", "joy", "bum", "rio", "yon", "dec", "leg", "put", "sue", "dim", "pet", "yaw", "nub", "bit", "bur", "sid", "sun", "oil", "red", "doc", "moe", "caw", "eel", "dix", "cub", "end", "gem", "off", "yew", "hug", "pop", "tub", "sgt", "lid", "pun", "ton", "sol", "din", "yup", "jab", "pea", "bug", "gag", "mil", "jig", "hub", "low", "did", "tin", "get", "gte", "sox", "lei", "mig", "fig", "lon", "use", "ban", "flo", "nov", "jut", "bag", "mir", "sty", "lap", "two", "ins", "con", "ant", "net", "tux", "ode", "stu", "mug", "cad", "nap", "gun", "fop", "tot", "sow", "sal", "sic", "ted", "wot", "del", "imp", "cob", "way", "ann", "tan", "mci", "job", "wet", "ism", "err", "him", "all", "pad", "hah", "hie", "aim", "ike", "jed", "ego", "mac", "baa", "min", "com", "ill", "was", "cab", "ago", "ina", "big", "ilk", "gal", "tap", "duh", "ola", "ran", "lab", "top", "gob", "hot", "ora", "tia", "kip", "han", "met", "hut", "she", "sac", "fed", "goo", "tee", "ell", "not", "act", "gil", "rut", "ala", "ape", "rig", "cid", "god", "duo", "lin", "aid", "gel", "awl", "lag", "elf", "liz", "ref", "aha", "fib", "oho", "tho", "her", "nor", "ace", "adz", "fun", "ned", "coo", "win", "tao", "coy", "van", "man", "pit", "guy", "foe", "hid", "mai", "sup", "jay", "hob", "mow", "jot", "are", "pol", "arc", "lax", "aft", "alb", "len", "air", "pug", "pox", "vow", "got", "meg", "zoe", "amp", "ale", "bud", "gee", "pin", "dun", "pat", "ten", "mob"};
    vector<string> input(std::begin(p), std::end(p));
    //vector<string> input;
    //copy(std::begin(p), std::end(p), input);
    printf("input has %lu elements\n", input.size());
    Solution so;
    vector<vector<string> > ans = so.findLadders(start, end, input);

    printf("=======done======\n");
    printVecVec(ans);
}

