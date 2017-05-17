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
#include <string.h>

using namespace std;
static bool pflag = true;

#define printf(fmt, args...) \
if(pflag) { \
printf(fmt, ##args); \
} \
else{ \
}

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
void printList(list<string> & lst)
{
    printf(" list= ");
    list<string>::iterator itr = lst.begin();
    while (itr!=lst.end()){
        printf(" %s ", (*itr).c_str());
	itr++;
    }

    printf("\n");
}

class Solution
{
public:
    vector<vector<string> > findLadders(string beginWord, string endWord, vector<string>& wordList) {
        {
            unsigned long curTime1 = curTimeMs();
	    //genRelationMatrix(beginWord, wordList);
            genRelation(beginWord, wordList);
            unsigned long curTime2 = curTimeMs();
            bool tmp=pflag;
            pflag=true;
            printf("======create Relation use %lu micro seconds\n", curTime2-curTime1);
            pflag=tmp;
        }
        {
            unsigned long curTime1 = curTimeMs();
            genGraph(beginWord, endWord, wordList, 0);
            unsigned long curTime2 = curTimeMs();
            bool tmp=pflag;
            pflag=true;
            printf("======create Graph use %lu micro seconds\n", curTime2-curTime1);
            pflag=tmp;
        }
        printf("======find solution for %s to %s finish\n", beginWord.c_str(), endWord.c_str());
	//std::copy(ansVec, ansList.begin(), ansList.
	return ansVec;
    }
private:
    int ** relationMatrix;
    string bWord;
    bool isConn(string &beginWord, string &word) {
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
    bool isCon1(int i, int j) {
        if (i<j)
            return relationMatrix[i][j];
        else
            return relationMatrix[j][i];
    }

    //vector<string> curAns;
    list<string> curAns1;
    vector<vector<string> > ansVec;
    //list<vector<string> > ansList;
    list<list<string> > ansList;

    //list<string> strqueue;
    list<int> stridxqueue;
    list<int> ongoingLayer;
    vector<string> wordToPr;
    void genRelationMatrix(string beginWord, vector<string> &wordList)
    {
	bWord=beginWord;
        wordToPr=wordList;
        int size=wordList.size();
        relationMatrix = new int *[size+1];
        for (int i=0;i<size+1;i++) {
            relationMatrix[i]=new int[size+1];
            memset(relationMatrix[i], -1, sizeof(int)*(size+1));
	}
    }
    bool isCon(int i, int j, vector<string> & wordList) {
	if (i>j) {
		int temp=i;
		i=j;
		j=temp;
	}
        //if (i<=j) {
	    if (relationMatrix[i][j]!=-1) {
		printf("alrady has it! %d and %d is %d\n", i, j, relationMatrix[i][j]);
            	return relationMatrix[i][j];
	    }
	    else {
		string k1=bWord;
		string k2=bWord;
		if (i!=0) k1=wordList[i-1];
		if (j!=0) k2=wordList[j-1];
		bool c= isConn(k1, k2);
		relationMatrix[i][j]=c;
		//printf("after calc, %s(%d) and %s(%d) is %d\n", k1.c_str(), i,k2.c_str(), j, relationMatrix[i][j]);
		return c;
	    }	
	//}
    }

    void genRelation(string beginWord, vector<string> &wordList)
    {
        bWord=beginWord;
	wordToPr=wordList;
        int size=wordList.size();
        relationMatrix = new int *[size+1];
        for (int i=0;i<size+1;i++) {
            relationMatrix[i]=new int[size+1];
	    int relationCount=0;
            memset(relationMatrix[i], 0, sizeof(int)*(size+1));
            for (int j=i;j<size+1;j++) {
                string k1;
                string k2;
                if (i==0)k1=bWord;
                else
                    k1=wordList[i-1];
                if (j==0)k2=bWord;
                else
                    k2=wordList[j-1];
                if (isConn(k1, k2)) {
                    //printf("set k1=%s, k2=%s [%d][%d]=1\n",k1.c_str(), k2.c_str(), i, j);
                    relationMatrix[i][j]=1;
		    relationCount++;
                }
            }
	    /*if (relationCount==0) {
		int index=i;
		if (index>0) index--;
		printf("[genRelation]: %s(%d) not connected to others, removeit\n", wordList[index].c_str(), i);
		wordList[index]="";
	    }*/
        }
        printf("    %s ", beginWord.c_str());
        for (int i=0;i<size;i++) {
                printf(" %s ", wordList[i].c_str());
        }
        printf("\n");
        int strlen=wordList[0].size();
        for (int i=0;i<size+1;i++) {
            if (0==i) {
                printf("%s ", beginWord.c_str());
	    }
            else {
                printf("%s ", wordList[i-1].c_str());
	    }

            for (int j=0;j<size+1;j++) {
                printf("  %d  ", relationMatrix[i][j]); 
            }
            printf("\n");
        }
        printf("====gen relation finish===\n");
    }
    void genGraph(string beginWord, string endWord, vector<string> & wordList, int depth)
    {
        bool inThisLayer=false;
        int count=0;
        {
            //vector<string> ans;
	    list<string> ans;
            ans.push_back(beginWord);
            ansList.push_back(ans);
            count++;
            //strqueue.push_back(beginWord);
            stridxqueue.push_back(0);
        }
        int lc =0;
        int ansCount=count;
        int index=0;
            while (0!=stridxqueue.size() || inThisLayer==false) {

                if (count==0) {
                    printf("-------layer %d finish-----\n", depth);
                    if (inThisLayer==true) return;
                    list<int>::iterator itr = ongoingLayer.begin();
                    while (itr!=ongoingLayer.end()) {
                        printf("%s is removed from wordList\n", wordList[*itr].c_str());
                        wordToPr[*itr]="";
                        itr++;
                    }
                    ongoingLayer.clear();

                    count=lc;
                    lc=0;
                    index=0;
                    depth++;
                }
                //string curWord=strqueue.front();
                //strqueue.pop_front();
                int curIndex=stridxqueue.front();
                string curWord=beginWord;
                if (curIndex!=0) curWord=wordList[curIndex-1];
                stridxqueue.pop_front();
                printf("process %s(%d) depth=%d, count=%d, lc=%d, index=%d\n", curWord.c_str(), curIndex, depth, count, lc, index);
                
		list<string> curAns=ansList.front();
		ansList.pop_front();
                count--;
                if (depth>wordList.size()) break;
                
                for (int i=0;i<wordToPr.size();i++) {
                    if (wordList[i]==beginWord) {
                        wordList[i]="";
			wordToPr[i]="";
			continue;
                    }
		    if (wordToPr[i]=="" || wordList[i]=="") continue;
                    printf("base %s(%d) depth=%d, processing %s(%d)\n", curWord.c_str(), curIndex, depth, wordList[i].c_str(), i);
                    //if (isCon(curIndex, i+1, wordList)) {
                    if (isCon1(curIndex, i+1)) {
                        if (endWord==wordList[i]) {
                            inThisLayer=true;
                        }
                        else {
                            //strqueue.push_back(wordList[i]);
                            stridxqueue.push_back(i+1);
                        }
                        ongoingLayer.push_back(i);
			list<string> ans = curAns;
			printf("base ans(%d)=: ", index);
			printList(ans);
			ans.push_back(wordList[i]);
                        ansList.push_back(ans);
			if (inThisLayer==true) {
				vector<string> fa(ans.begin(), ans.end());
				ansVec.push_back(fa);
			}
			printf("new ans(%d)=: ", index);
			printList(ans);
                        printf("lc=%d, add %s(%d) -->%s(%d)\n", lc, curWord.c_str(), curIndex, wordList[i].c_str(), i);

                        index++;
                        lc++;
			
                    }
                }
            }
        //ansVec.pop_back();
	
        //}
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
    string end5="bax";
    string p5[]= {"ted","tex","tax","tad","rex","tix", "bax"};
    vector<string> input1(std::begin(p1), std::end(p1));
    vector<string> input2(std::begin(p2), std::end(p2));
    vector<string> input3(std::begin(p3), std::end(p3));
    vector<string> input4(std::begin(p4), std::end(p4));
    vector<string> input5(std::begin(p5), std::end(p5));
    //vector<string> input;
    //copy(std::begin(p), std::end(p), input);
    Solution so;
    int choose = 2;
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
        printf("input has %lu elements, begin=%s, end=%s\n", input2.size(), start2.c_str(), end2.c_str());
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

