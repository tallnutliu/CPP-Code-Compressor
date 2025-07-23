#include <bits/stdc++.h>
using namespace std;
string rfile,wfile,ori,tmp,ans;
vector<string> lines;
struct word
{
    bool success;
    //type=0表示变量名，type=1表示符号
    int type;
    int l;
    int r;
} tmp1,tmp2;
bool isalpha_(char c) { return isalpha(c) || c == '_'; }
bool isspecial(char c) { return (!isalpha_(c)) && (!isdigit(c)) && (!isspace(c)); }
word getword(int lineidx,int begpos)
{
    word ret;
    //跳过空格
    while (begpos < lines[lineidx].size() && isspace(lines[lineidx][begpos]))
        begpos++;
    if (begpos >= lines[lineidx].size())
    {
        ret.success = false;
        return ret;
    }
    else ret.success = true;
    ret.l = begpos;
    //判断：如果开头就是alpha_
    if (isalpha_(lines[lineidx][begpos]))
    {
        ret.type = 0;
        for (int i = begpos + 1;i < lines[lineidx].size();i++)
            if (!(isalpha_(lines[lineidx][i]) || isdigit(lines[lineidx][i])))
            {
                ret.r = i - 1;
                return ret;
            }
    }
    else
    {
        ret.type = 1;
        for (int i = begpos + 1;i < lines[lineidx].size();i++)
            if (!isspecial(lines[lineidx][i]) || isspace(lines[lineidx][i]))
            {
                ret.r = i - 1;
                return ret;
            }
    }
    ret.r = lines[lineidx].size() - 1;
    return ret;
}
int main()
{
    cin >> rfile >> wfile;
    ifstream file(rfile.c_str());
    while (getline(file,tmp))
    {
        ori += tmp;
        ori += '\n';
    }
    file.close();
    //删除多行注释
    while (ori.find("/*") != string::npos)
        ori.erase(ori.find("/*"),ori.find("*/") - ori.find("/*") + 2);
    //重新分割成行
    for (int i = 0;i < ori.size();)
    {
        int j = i;
        for (;j < ori.size() && ori[j] != '\n';j++);
        lines.push_back(ori.substr(i,j - i));
        i = j + 1;
    }
    //删除单行注释
    for (int i = 0;i < lines.size();i++)
        for (int j = 0;j < ((int)(lines[i].size()) - 1);j++)
            if (lines[i][j] == '/' && lines[i][j + 1] == '/')
            {
                lines[i] = lines[i].substr(0,j);
                break;
            }
    //删除行首空格和行尾空格
    for (int i = 0;i < lines.size();i++)
    {
        while (lines[i].size() && isspace(lines[i][0]))
            lines[i].erase(lines[i].begin());
        while (lines[i].size() && isspace(lines[i][lines[i].size() - 1]))
            lines[i].erase(lines[i].end() - 1);
    }
    //删除空行
    for (int i = 0;i < lines.size();)
        if (lines[i].size() == 0)
            lines.erase(lines.begin() + i);
        else
            i++;
    //正式处理
    bool last_word_is_alphanum;
    for (int i = 0;i < lines.size();i++)
    {
        vector<word> words;
        for (int j = 0;;)
        {
            word wtmp = getword(i,j);
            if (wtmp.success)
            {
                words.push_back(wtmp);
                j = wtmp.r + 1;
            }
            else break;
        }
        if (words.size() == 0) continue;
        string lineans;
        if (i > 0 && last_word_is_alphanum && words[0].type == 0)
            lineans += ' ';
        lineans += lines[i].substr(words[0].l,words[0].r - words[0].l + 1);
        if (words.size() == 1) last_word_is_alphanum = (words[0].type == 0 || isdigit(lines[i][words[0].r]));
        for (int j = 1;j < words.size();j++)
        {
            //两个英文单词之间要空格
            if (words[j].type == 0 && words[j - 1].type == 0)
                lineans += ' ';
            //词和数字也要空格
            else if (words[j - 1].type == 0 && isdigit(lines[i][words[j].l]))
                lineans += ' ';
            //一对单引号也要空格，而且可能空多个格子
            else if (lines[i][words[j - 1].r] == '\'' && lines[i][words[j].l] == '\'')
            {
                for (int k = words[j - 1].r + 1;k < words[j].l;k++)
                    lineans += ' ';
            }
            //双引号同理
            else if (lines[i][words[j - 1].r] == '\"' && lines[i][words[j].l] == '\"')
            {
                for (int k = words[j - 1].r + 1;k < words[j].l;k++)
                    lineans += ' ';
            }
            lineans += lines[i].substr(words[j].l,words[j].r - words[j].l + 1);
            if (j == words.size() - 1) last_word_is_alphanum = (words[j].type == 0 || isdigit(lines[i][words[j].r]));
        }
        if (lines[i][0] == '#')
        {
            ans += lineans;
            ans += '\n';
        }
        else ans += lineans;
    }
    if (ans[0] == '\n') ans.erase(ans.begin());
    //写入文件
    ofstream file2(wfile.c_str());
    file2 << ans;
    file2.close();
}
