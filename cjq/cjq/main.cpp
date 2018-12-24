#include <iostream>
#include <string>
#include "skill.h"
#include "buff.h"
#include "time.h"
#include <stdlib.h>

using namespace std;

void update(skill_all* cjq_skill, buff_all* cjq_buff, string str, double gcd, bool Verdict, bool S_of_J)
{
	cjq_skill->update(str, gcd);
	cjq_buff->update(gcd, Verdict, S_of_J);
}

int Random_judge(double rate)
{
	int rate_int = int(rate * 1000);
	int ran = rand() % 1000 + 1;	
	if (rate_int >= ran) return 1;
	else return 0;
}

int main()
{
	srand((unsigned)time(NULL));

	double critical = 0.227;  //属性
	double haste = 0.119;
	double mastery = 0.294;
	double versatile = 0.017;
	double gcd = 1.5 / (1 + haste);

	skill_all cjq_skill = skill_all(gcd);
	buff_all cjq_buff = buff_all();

	double time = 0;
	double autoattack = 0;
	double autoattackdamage = 3110;
	double autoattckspeed = 3.6 / (1 + haste);
	double damage = 0;
	int energy = 0;   //豆子

	int count_cri = 0; //统计暴击
	int count = 0;

	int count_ver = 0; //统计裁决
	int count_ver_cri = 0;
	int count_just_ver = 0;
	double damage_ver = 0;

	double damage_attack = 0;
	double damage_sword = 0;
	double damage_judgement = 0;
	double damage_strike = 0;
	double damage_ash = 0;

	while (time < 300)
	{
		bool gcd_occupied = false;	
		double current_damage = 0;
		double current_critical = critical;
		double damage_buff = 1;
		string current_skill_name = "";
		if (cjq_buff.get_cooldown("Avenging_Wrath") == 0) {
			gcd_occupied = true; cjq_buff.set_buff("Avenging_Wrath");
			cout << "Avenging_Wrath" << endl;
		}
		cout << "Buff last time:"<< cjq_buff.get_time("Avenging_Wrath") << " " << cjq_buff.get_time("Just_Verdict")<< " " << cjq_buff.get_time("Holy_DivinePurpose") << " " << cjq_buff.get_time("Judgement") << " " << cjq_buff.get_time("Blade_of_Fury") << endl;
		bool DivinePurpose = false;
		if (cjq_buff.get_time("Holy_DivinePurpose") > 0) DivinePurpose = true;
		if (energy >= 3 || DivinePurpose)
		{
			if (cjq_buff.get_time("Just_Verdict") < gcd || energy==5) {
				gcd_occupied = true;
				current_skill_name = "Templars_Verdict";							
			}
			else
			{
				if (!gcd_occupied && (energy == 4) && (cjq_skill.get_cooldown("Sword_of_Justice") == 0))
				{
					gcd_occupied = true;
					current_skill_name = "Templars_Verdict";
				}
				if (!gcd_occupied && (energy < 4 ) && (cjq_skill.get_cooldown("Sword_of_Justice") == 0))
				{
					gcd_occupied = true;
					current_skill_name = "Sword_of_Justice";
				}
				if (!gcd_occupied && (cjq_skill.get_cooldown("Judgement") == 0))
				{
					gcd_occupied = true;
					current_skill_name = "Judgement";
				}
				if (!gcd_occupied && (cjq_skill.get_max_charge("Crusader_Strike") >= 1))
				{
					gcd_occupied = true;
					current_skill_name = "Crusader_Strike";
				}
				if (!gcd_occupied)
				{
					if (DivinePurpose || (cjq_skill.get_cooldown("Sword_of_Justice") < gcd) || (cjq_skill.get_cooldown("Judgement") < gcd) || (cjq_skill.get_cooldown("Crusader_Strike") < gcd))
					{
						gcd_occupied = true;
						current_skill_name = "Templars_Verdict";
					}					
				}
			}
		}
		else
		{
			if (!gcd_occupied && (cjq_skill.get_cooldown("Wake_of_Ashes") == 0))
			{
				gcd_occupied = true;
				current_skill_name = "Wake_of_Ashes"; 
			}
			if (!gcd_occupied && (cjq_skill.get_cooldown("Sword_of_Justice") == 0))
			{
				gcd_occupied = true;
				current_skill_name = "Sword_of_Justice";
			}
			if (!gcd_occupied && (cjq_skill.get_cooldown("Judgement") == 0))
			{
				gcd_occupied = true;
				current_skill_name = "Judgement";
			}
			if (!gcd_occupied && (cjq_skill.get_max_charge("Crusader_Strike") >=1))
			{
				gcd_occupied = true;
				current_skill_name = "Crusader_Strike";
			}
		}

		if (current_skill_name != "") 
			if (current_skill_name!="Templars_Verdict" || !DivinePurpose )
				energy = energy + cjq_skill.get_gain(current_skill_name);
		if (energy > 5) energy = 5;

		if (current_skill_name != "") current_damage = cjq_skill.get_damage(current_skill_name)*(1 + cjq_skill.get_holy_damage(current_skill_name)*mastery);	

		bool Verdict = false;
		if (current_skill_name == "Templars_Verdict") Verdict = true;		
		bool S_of_J = false;
		if (current_skill_name == "Sword_of_Justice") S_of_J = true;
		if (current_skill_name == "Templars_Verdict")
		{
			if (cjq_buff.get_time("Just_Verdict") > 0) {
				count_just_ver ++ ; damage_buff = damage_buff*(1 + cjq_buff.get_add_d_v("Just_Verdict"));
			}
			if (cjq_buff.get_time("Judgement") > 0) damage_buff = damage_buff*(1 + cjq_buff.get_add_d_v("Judgement"));
			cjq_buff.set_buff("Just_Verdict");
		}		
		if (current_skill_name == "Sword_of_Justice")
			if (cjq_buff.get_time("Blade_of_Fury") > 0)	
				damage_buff = damage_buff*(1 + cjq_buff.get_add_d_v("Blade_of_Fury"));							
		if (current_skill_name == "Judgement") cjq_buff.set_buff("Judgement");		
		update(&cjq_skill, &cjq_buff, current_skill_name, gcd, Verdict, S_of_J);

		//触发神圣意志
		if (current_skill_name == "Templars_Verdict")
		{
			int div = Random_judge(0.15);
			if (div == 1) cjq_buff.set_buff("Holy_DivinePurpose");
		}

		//计算buff
		if (cjq_buff.get_time("Avenging_Wrath") > 0)  //翅膀
		{
			damage_buff = damage_buff*(1+cjq_buff.get_add_d("Avenging_Wrath"));
			current_critical = current_critical + cjq_buff.get_add_c("Avenging_Wrath");
		}
		damage_buff = damage_buff*(1 + versatile);    //全能

		int cri = 0;                                  //暴击 
		if (current_skill_name != "")
		{
			cri = Random_judge(current_critical);
			damage_buff = damage_buff * (1 + cri);
		}
		current_damage = current_damage* damage_buff;
		damage = damage + current_damage;
		if (current_skill_name == "Judgement") damage_judgement = damage_judgement + current_damage;
		if (current_skill_name == "Sword_of_Justice") damage_sword = damage_sword + current_damage;
		if (current_skill_name == "Crusader_Strike") damage_strike = damage_strike + current_damage;
		if (current_skill_name == "Wake_of_Ashes") damage_ash = damage_ash + current_damage;
		cout << "Critical:" << current_critical << endl;
		cout << (int)time/60 << ":" << (int)time % 60 << current_skill_name << " " << current_damage << " " << cri << " " << energy << " " << "\n";	
		cout << cjq_skill.get_cooldown("Judgement") << " " << cjq_skill.get_cooldown("Sword_of_Justice") << " " << cjq_skill.get_cooldown("Crusader_Strike") << endl;
		if (current_skill_name != "")
		{
			count++;
			if (cri != 0) count_cri++;
			if (current_skill_name == "Templars_Verdict")
			{
				count_ver++;
				if (cri != 0) count_ver_cri++;
				damage_ver = damage_ver + current_damage;
			}
		}

		//自动攻击

		if (autoattack < gcd)
		{
			cri = 0;                                  //暴击 			
			cri = Random_judge(current_critical);						
			if (cjq_buff.get_time("Avenging_Wrath") > 0)
			{
				cout << "autoattack:" << autoattackdamage*(1 + cjq_buff.get_add_d("Avenging_Wrath"))*(1+cri)<<endl;
				damage = damage + autoattackdamage *(1 + cjq_buff.get_add_d("Avenging_Wrath"))*(1+cri);
				damage_attack = damage_attack + autoattackdamage *(1 + cjq_buff.get_add_d("Avenging_Wrath"))*(1 + cri);
			}
			else
			{
				cout << "autoattack:" << autoattackdamage*(1+cri) << endl;
				damage = damage + autoattackdamage*(1+cri);
				damage_attack = damage_attack + autoattackdamage *(1 + cjq_buff.get_add_d("Avenging_Wrath"))*(1 + cri);
			}

			//触发愤怒之剑
			int as = 0;
			as = Random_judge(0.2);
			if (as)
			{
				cjq_skill.set_skill("Sword_of_Justice");
				cjq_buff.set_buff("Blade_of_Fury");
				cout << "Blade_of_Fury" << endl;
			}
		}

		time += gcd;
		cout << endl;
	}
	cout << damage / time << "\n";
	cout << "Critical_rate: "<< count_cri*1.0/count << "\n";
	cout << "Average Verdict:" << count_ver << " " << damage_ver / count_ver << " " << count_ver_cri << " " << count_just_ver << "\n";

	cout << "Skada:" << endl;
	cout << "Templar's Verdict: " << damage_ver/time << " " << damage_ver / damage << endl;
	cout << "Auto attack: " << damage_attack/time << " " << damage_attack / damage << endl;
	cout << "Sword of Justice: " << damage_sword / time << " " << damage_sword / damage << endl;
	cout << "Judgement: " << damage_judgement / time << " " << damage_judgement / damage << endl;
	cout << "Crusader Strike: " << damage_strike / time << " " << damage_strike / damage << endl;
	cout << "Wake of Ashes: " << damage_sword / time << " " << damage_sword / damage << endl;

	system("pause");
}