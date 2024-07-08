#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <exception>
#include <cctype>
#include <algorithm>

//using MultiType = std::variant<int, std::string, double>;

//using filestruct = std::map< std::string, std::map<std::string,MultiType>>;
using filestruct = std::map< std::string, std::map<std::string,std::string>>;


class ini_parser{
private:
    std::string filename_;
    filestruct fstruct_;
    //public:
    std::string eraseWS(const std::string& s){
        std::string res = s;
        res.erase(std::remove_if(res.begin(), res.end(), ::isspace),
                  res.end());
        return res;
    }
    void split_in_two(const std::string& s, std::string& before, std::string& after, const char& del)
    {
        std::stringstream ss(s);
        before = "";
        after = "";
        bool got_before = false;
        while (!ss.eof()) {
            if(!got_before){
                getline(ss, before, del);
                got_before = true;
            }else
                getline(ss, after);
        }
    }
    void build_fstruct(){
        std::ifstream infile(filename_);
        if(!infile){
            throw std::runtime_error("Error opening file\nException thrown");
        }
        std::string line;
        std::string before;
        std::string after;
        std::string section;
        std::string bad_syntax_comment = "";
        int count = 1;
        while(getline(infile,line)){
            split_in_two(line,before,after,';');
            if(!eraseWS(before).empty()){
                line = before;
                split_in_two(line,before,after,'[');
                //before = eraseWS(before);
                if(eraseWS(before).empty()){
                    line = after;
                    split_in_two(line,before,after,']');
                    if(line != before){ //']' HAS BEEN FOUND
                        section = before;
                    }else bad_syntax_comment = "No closing ']' found\n";
                }else if(line == before){ //'[' HASN'T BBEN FOUND
                    //before = "";
                    split_in_two(line,before,after,'=');
                    if(before != line && !section.empty() && !after.empty()){
                        fstruct_[section][before] = after;
                    }else if(before==line){ //NONE OF ; OR [ OR = HAS BEEN FOUND
                        bad_syntax_comment = "None of ';' or '[' or '=' has been found among other symbols";
                    }
                }else bad_syntax_comment = "Symbols before opening '[' found\n";
            }//else std::cout << "Line " << count << " is a comment or empty\n";
            if(bad_syntax_comment != ""){
                std::cout << "Bad syntax in line " << count << ": " << bad_syntax_comment << "\n";
                bad_syntax_comment = "";
            }
            count++;
        }
    }

public:
    template<typename T>
    T get_value(std::string str){
        std::string section;
        std::string key;
        split_in_two(str, section, key, '.');
        if(fstruct_.count(section)==0) {
            std::cout << "Section not found\n";
            std::cout << "Perhaps you meant one of the non-empty sections available:\n";
            for(const auto& sec : fstruct_){
                std::cout << sec.first << "\n";
            }
            throw std::runtime_error("Exception thrown");
        }
        else if(fstruct_[section].count(key)==0) {
            std::cout << "Variable name not found\n";
            std::cout << "Perhaps you meant one of the variables available in section \"" << section << "\":\n";
            for(const auto& record : fstruct_[section]){
                std::cout << record.first << "\n";
            }
            throw std::runtime_error("Exception thrown");
        }
        else{
            T res{};
            std::istringstream in(fstruct_[section][key]);
            in >> res;
            if(in.fail()){
                throw std::runtime_error("Wrong type!\nException thrown\n");
            }
            return res;
        }
    }
    ini_parser(std::string filename):filename_(filename){
        build_fstruct();
    }
};


int main(){
    try{
        ini_parser parser("file.ini");
        auto value = parser.get_value<double>("Section1.var1");
        //  WRONG SECTION NAME
        //auto value = parser.get_value<double>("Section10.var1");
        //  WRONG VALUE NAME
        //auto value = parser.get_value<double>("Section1.var10");
        //  WRONG TYPE
        //auto value = parser.get_value<int>("Section1.var2");
        std::cout << "Value requested is " << value << "\n";
    }catch(const std::exception& e){
        std::cout << e.what();
    }

    return  0;
}
