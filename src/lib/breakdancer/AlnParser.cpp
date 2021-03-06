#include "AlnParser.h"

using namespace std;
extern string platform;
//extern std::map<char *, std::string> readgroup_platform;

vector<string> AlnParser(bam1_t *b, string format, string alt, map<string, string> &readgroup_platform, int same_tid, string platform){
	string ori;
	string readgroup;
	vector<string> return_;
	if(! format.compare("sam")){
		uint32_t flag = b->core.flag;
		ori = (flag&0x0010 /*|| str_flag.find("r") != string::npos*/)?"-":"+";
		b->core.flag = 0;
		
		if(uint8_t *tmp = bam_aux_get(b, "RG")){
			readgroup = bam_aux2Z(tmp);
			//platform = readgroup_platform[readgroup];
			platform = (readgroup_platform.count(readgroup)>0)?readgroup_platform[readgroup]:"illumina";
		}
		else{
			readgroup = "";
		}
		
		
		string string2_cmp = "AQ:i";
		
		if(alt.empty()){
			if(uint8_t *tmp = bam_aux_get(b, "AQ"))
				b->core.qual = bam_aux2i(tmp);
			else if(uint8_t *tmp = bam_aux_get(b, "AM"))
				b->core.qual = bam_aux2i(tmp);					
		}
		
		// convert to Maq flag
		if(uint8_t *tmp = bam_aux_get(b, "MF"))
			b->core.flag = bam_aux2i(tmp);
		else{
			if(flag & 0x0400 /*|| str_flag.find("d")!=string::npos*/)
				b->core.flag = 0;
			else if(flag & 0x0001 /*|| str_flag.find("p")*/){
				string ori2;
				ori2 = (flag & 0x0020 /*|| str_flag.find("R")*/)?"-":"+";
				if(flag & 0x0004 /*|| str_flag.find("u")*/)
					b->core.flag = 192;
				else if(flag & 0x0008 /*|| str_flag.find("U")*/)
					b->core.flag = 64;
				else if(same_tid != 1)
					b->core.flag = 32;
				else if(flag & 0x0002 /*|| str_flag.find("P")*/){
					if(platform.find("solid")!=string::npos) // need to work on insensitive case
						b->core.flag = 18;
					else{
						if(b->core.pos < b->core.mpos)
							b->core.flag = (ori.compare("+") == 0)?18:20;
						else
							b->core.flag = (ori.compare("+") == 0)?20:18;
					}
				}
				else{
					if(platform.find("solid")!=string::npos) {// insensitive case 
						if(ori.compare(ori2) != 0)
							b->core.flag = (ori2.compare("+") == 0)?1:8;
						else if(ori.compare("+") == 0){
							if(flag & 0x0040)
								b->core.flag = (b->core.pos < b->core.mpos)?2:4;
							else
								b->core.flag = (b->core.pos > b->core.mpos)?2:4;
						}
						else{
							if(flag & 0x0040)
								b->core.flag = (b->core.pos > b->core.mpos)?2:4;
							else
								b->core.flag = (b->core.pos < b->core.mpos)?2:4;
						}
						//else
							//b->core.flag = 2;
					}
					else{
						if(ori.compare(ori2) == 0)
							b->core.flag = (ori2.compare("+") == 0)?1:8;
						else if((b->core.mpos > b->core.pos && (ori.compare("-")) == 0) || (b->core.pos > b->core.mpos && (ori.compare("+") == 0)))
							b->core.flag = 4;
						else
							b->core.flag = 2;
					}
				}
			}
		}
	}
	return_.push_back(readgroup);
	return_.push_back(ori);
	return return_;
}




























