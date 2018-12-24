#pragma once
#include <iostream>
using namespace std;

struct skill {
	string name;        //技能名称
	double maxcooldown; //最大冷却时间
	double cooldown;    //冷却时间
	double damage;      //伤害
	int gain;           //神圣能量获得
	int max_charge;     //最大充能	
	int holy_damage;    //是否是神圣伤害
};

class skill_all : skill{	
	skill Templars_Verdict{ "Templars_Verdict",0,0,10197,-3,0,1 }; //圣殿骑士的裁决
	skill Crusader_Strike{ "Crusader_Strike",6,0,4020,1,2,0 };     //十字军打击
	skill Sword_of_Justice{ "Sword_of_Justice",11,0,7620,2,1,0 };  //公正之剑
	skill Judgement{ "Judgement",12,0,4793,1,1,1 };                //审判
	skill Wake_of_Ashes{ "Wake_of_Ashes",45,0,11037,5,1,1 };       //灰烬觉醒 

	skill* get_name(string str) {
		if (str == "Templars_Verdict") return &Templars_Verdict;
		if (str == "Crusader_Strike") return &Crusader_Strike;
		if (str == "Sword_of_Justice") return &Sword_of_Justice;
		if (str == "Judgement") return &Judgement;
		if (str == "Wake_of_Ashes") return &Wake_of_Ashes;
	};

	void cutdown(skill* s, double gcd)
	{
		if (s->name == "Crusader_Strike") {
			if (s->cooldown > gcd) s->cooldown = s->cooldown - gcd;
			else if (s->max_charge == 1) {
				s->max_charge++;
				s->cooldown = 0;
			}
			else if (s->max_charge == 0)
			{
				s->max_charge++;
				s->cooldown = s->cooldown - gcd + s->maxcooldown;
			}
		}
		else if (s->cooldown > gcd) s->cooldown = s->cooldown - gcd; 
		else s->cooldown = 0;		
	};

	void setcooldown(skill* s,double gcd) {
		s->maxcooldown = s->maxcooldown*gcd / 1.5;
	};

public:
	skill_all(double gcd) {
		setcooldown(&Crusader_Strike, gcd);
		setcooldown(&Sword_of_Justice, gcd);
		setcooldown(&Judgement, gcd);			
	};

	double get_cooldown(string str) { skill* b = get_name(str); return b->cooldown; };
	double get_damage(string str) { skill* b = get_name(str); return b->damage; };
	int get_gain(string str) { skill* b = get_name(str); return b->gain; };
	int get_max_charge(string str) { skill* b = get_name(str); return b->max_charge; };
	bool get_holy_damage(string str) { skill* b = get_name(str); return b->holy_damage; };

	void update(string str, double gcd)
	{
		if (str != "")
		{
			skill* s = get_name(str);
			if (s->name == "Crusader_Strike")
			{
				if (s->max_charge == 2)	s->cooldown = s->maxcooldown;
				s->max_charge--;
			}
			else
			{
				s->cooldown = s->maxcooldown;
			}
		}		
		cutdown(&Crusader_Strike,gcd);
		cutdown(&Sword_of_Justice, gcd);
		cutdown(&Judgement, gcd);
		cutdown(&Wake_of_Ashes, gcd);		
	}

	void set_skill(string str)
	{
		if (str == "Sword_of_Justice")
		{
			skill* s = get_name(str);
			s->cooldown = 0;
		}
	}
};