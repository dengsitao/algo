#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <set>
#include <list>
#include <climits>

using namespace std;

void printVecVec(vector<vector<string> >& vecvec)
{
    for (int i=0;i<vecvec.size();i++) {
        printf(" vec[%d]= ", i);
        for (int j=0;j<vecvec[i].size();j++) {
            printf(" %s ", vecvec[i][j].c_str());
        }
        printf("\n");
    }
}

class Solution {
public:
    vector<vector<string> > findLadders(string beginWord, string endWord, vector<string>& wordList) {
        int size = wordList.size();
        //vector<vector<string> > answers;
        string startWord=beginWord;
        curAns.push_back(beginWord);
        findPath(beginWord, endWord, wordList);
        return ansVec;
    }
    Solution() {
        //ansIndex=0;
        minDepth=INT_MAX;
        curDepth=0;
        //vector<string> vec;
        //ansVec.push_back(vec);
    }
private:
    bool isCon(string beginWord, string word) {
        int diff=0;
        if (beginWord.size()!=word.size()) return false;
        for (int i=0;i<beginWord.size();i++) {
            if (beginWord[i]!=word[i]) diff++;
            if (diff > 1) return false;//more than one diff
        }
        if (diff == 1)//must be one diff
            return true;
        else //same word
            return false;
    }
    set<string> used;
    vector<vector<string> > ansVec;
    vector<string> curAns;
    int minDepth;
    int curDepth;
    void findPath(string startWord, string endWord, vector<string>& wordList)
    {
        //printf("------findPath for %s [%d]begin --\n", startWord.c_str(), curDepth);
        curDepth++;
        used.insert(startWord);
        if (curDepth <= minDepth) {
            for (int i=0;i<wordList.size();i++) {
                if (used.find(wordList[i])!=used.end() || curDepth > minDepth) {
                    //printf("%s is used, ignore it\n", startWord.c_str());
                    continue;
                }
                
                if (isCon(startWord, wordList[i])) {
                    //printf("push %s into curAns\n", wordList[i].c_str());
                    curAns.push_back(wordList[i]);
                    if (endWord==wordList[i]) {
                        if (minDepth == curDepth) {
                            //printf("find one answer with min=%d == curmin\n", minDepth);
                            ansVec.push_back(curAns);
                            printVecVec(ansVec);
                            curAns.pop_back();
                            break;
                        }
                        else if (minDepth > curDepth) {
                            //printf("find one answer with depth=%d < %d\n", curDepth, minDepth);
                            minDepth = curDepth;
                            ansVec.clear();
                            ansVec.push_back(curAns);
                            printVecVec(ansVec);
                        }
                        else {
                            //printf("find one answer with depth=%d > %d\n",curDepth, minDepth);
                            curAns.pop_back();
                            break;
                        }
                    }
                    else {
                        findPath(wordList[i], endWord, wordList);
                    }
                    curAns.pop_back();
                }

            }
        }
        else {
            //printf("curDepth=%d > minDepth=%d, return\n", curDepth, minDepth);
        }

        curDepth--;
        used.erase(startWord);
        //printf("------findPath for %s end----\n", startWord.c_str());
        return;
    }
};

int main(int argc, char * argv[])
{
	string p[] = {"si","go","se","cm","so","ph","mt","db","mb","sb","kr","ln","tm","le","av","sm","ar","ci","ca","br","ti","ba","to","ra","fa","yo","ow","sn","ya","cr","po","fe","ho","ma","re","or","rn","au","ur","rh","sr","tc","lt","lo","as","fr","nb","yb","if","pb","ge","th","pm","rb","sh","co","ga","li","ha","hz","no","bi","di","hi","qa","pi","os","uh","wm","an","me","mo","na","la","st","er","sc","ne","mn","mi","am","ex","pt","io","be","fm","ta","tb","ni","mr","pa","he","lr","sq","ye"};
	vector<string> input(std::begin(p),std::end(p));
	//vector<string> input;
	//copy(std::begin(p), std::end(p), input);
	printf("input has %lu elements\n", input.size());
	Solution so;
	string start="qa";
	string end="sq";
	vector<vector<string> > ans = so.findLadders(start, end, input);
	printVecVec(ans);
}

