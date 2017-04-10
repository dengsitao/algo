#include "stdio.h"
#include <vector>
#include <list>
#include <string>
using namespace std;

class Solution {

public:

    vector<string> fullJustify(vector<string>& words, int maxWidth) {

        list<string> strlist;

        int count=0;

        int len=0;

        int size=words.size();

        int total=0;

        vector<string> result;

        //if (maxWidth==0)return result;

        printf("len=%d, %d\n", string("").size(), string(" ").size());

        while(total<size) {

            string a=words[total];

            while(len+a.size()<=maxWidth) {

                //if (len+a.size()<maxWidth)

                //    a.append(" ");

                strlist.push_back(a);

                total++;

                len+=a.size();

                count++;

                printf("adding %s to list, next is %s\n", a.c_str(), words[total].c_str());

                if (total >=size) break;

                a=words[total];

                if (len+a.size()<maxWidth) {

                    strlist.back().append(" ");

                    len++;

                }

            }

            if (count==0) return result;

            //return result;

            

            list<string>::iterator itr = strlist.begin();

            

            //while(itr != strlist.end()) {

            //    (*itr).append(" ");

            //    itr++;

            //}

            //if (count!=1) {

                //strlist.back().pop_back();

                //len=len-1;

                //printf("here len=%d\n", len);

                //len--;

            //}

            //len--;

            //(*itr).pop_back();

            count--;

            printf("count=%d, size=%d, len=%d, total=%d\n",count, size, len, total);

            

            int blanks = maxWidth-len;

            int blanks4each = 0;

            if (count!=0)

                blanks4each =blanks/count;

            printf("blanks=%d, b4e=%d\n", blanks, blanks4each);

            if (blanks4each!=0) {

                string strblanks;

                while(blanks4each!=0) {

                    strblanks.append(" ");

                    blanks4each--;

                }

                itr = strlist.begin();

                while(itr != strlist.end()) {

                    itr++;

                    if (itr == strlist.end()) break;

                    else itr--;

                    (*itr).append(strblanks);

                    itr++;

                }

            }

            

            printf("adding %d blanks for each string done\n", blanks4each);

            int blanksleft = 0;

            if (count==0) blanksleft=blanks;

            else blanksleft = blanks%count;

            printf("adding %d extra blanks\n", blanksleft);

            

            if (blanksleft!=0) {

                while(blanksleft>0){

                    list<string>::iterator itr = strlist.begin();

                    (*itr).append(" ");

                    itr++;

                    blanksleft--;

                }

            }

            

            string res;

            itr = strlist.begin();

            while(itr != strlist.end()) {

                res.append((*itr));

                itr++;

            }

            printf("result.size=%d, res=\"%s\" total=%d, size=%d\n", result.size(), res.c_str(), total, size);

            result.resize(total);

            result.push_back(res);

            len=0;

            count=0;

            strlist.clear();

            printf("done\n");

            

        }

        return result;

    }

};

int main()
{
	Solution s;
	vector<string> vec;
	//vec.push_back("");
	//vector<string> vec;
	vec.push_back("a");
	vec.push_back("b");
	vec.push_back("c");
	vec.push_back("d");
	vec.push_back("e");
	s.fullJustify(vec, 3);
	vec.clear();
	vec.push_back("");
	s.fullJustify(vec, 2);
	return 0;
}
