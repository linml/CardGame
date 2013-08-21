//
//  CConfigResourceLoad.cpp
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#include "CConfigResourceLoad.h"
#include "CCard.h"
#include "gameConfig.h"
#include "gameTools.h"
#include "CGamePlayerStruct.h"
#include "CSkillData.h"

bool CConfigResourceLoad::loadCardInfo(map<int,CCard *> &hashmapAllCard, const char *cardFileName)
{
    
    
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(cardFileName);
    CCArray *vKeyArray=directory->allKeys();
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *cardDirector=(CCDictionary*)(directory->objectForKey(key->m_sString));
        CCard *card=new CCard;
        card->m_icard_id=GameTools::intForKey("card_id", cardDirector);
        card->m_scard_name=string(GameTools::valueForKey("card_name", cardDirector));
        card->m_ccard_next=GameTools::intForKey("card_next", cardDirector);  ///背景底色
        card->m_sicard_star=GameTools::intForKey("card_star", cardDirector);
        card->m_icard_stirps=GameTools::intForKey("card_stirps", cardDirector);   //种族
        card->m_icard_suit=GameTools::intForKey("card_suit", cardDirector);      //随机数值
        card->m_icard_leadership=GameTools::intForKey("card_leadership", cardDirector);
        card->m_icard_exp=GameTools::intForKey("card_exp", cardDirector);    //吃掉该卡牌的 经验
        card->m_icard_price=GameTools::intForKey("card_price", cardDirector);  //卖掉该卡佩的 价格
      //  card->m_ileve_max=GameTools::intForKey("card_leve_max", cardDirector);   //等级最高级别
        card->m_icard_attack=GameTools::intForKey("card_attack", cardDirector); //攻击力
        card->m_icard_defend=GameTools::intForKey("card_defend", cardDirector); //防御力
        card->m_icardhp=GameTools::intForKey("card_hp",cardDirector);// 卡牌的总的HP
        card->m_iusually_attack=GameTools::intForKey("usually_attack", cardDirector);
        card->m_iskillLine=GameTools::intForKey("skill_energy", cardDirector);
        card->m_iskillHelp=GameTools::intForKey("skill_help", cardDirector);
        card->m_iskillDead=GameTools::intForKey("skill_dead", cardDirector);
        card->m_iskillBuff=GameTools::intForKey("skill_buff", cardDirector);
        card->m_scard_tips=string(GameTools::valueForKey("card_tips", cardDirector));
        card->m_scard_resources=GameTools::valueForKey("card_resources", cardDirector);
        card->m_scard_head=GameTools::valueForKey("card_head", cardDirector);
        card->m_scard_ground=GameTools::valueForKey("card_ground", cardDirector);
        card->m_scard_role=GameTools::valueForKey("card_role", cardDirector);
        
        // change by phileas:
        int index = card->m_sicard_star -1;
        index = abs(index) > 7 ? 7: abs(index);
        card->m_ileve_max = g_aMaxLevel[index];
        CCLog("the %i card's next card id: %d", card->m_icard_id, card->m_ccard_next);
        // change end:
        hashmapAllCard[key->intValue()]=card;
    }
    cout<<"card plist size ="<<hashmapAllCard.size()<<endl;
    if(hashmapAllCard.size()>0)
    {
        return true;
    }
    return false;
}
bool CConfigResourceLoad::loadPlayerLevelInfo(vector<SLevelPlayer *> *vPlayerLevel, const char *playerFileName)
{
    
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(playerFileName);
    CCArray *vKeyArray=directory->allKeys();
    for (int i=0; i<vKeyArray->count(); i++)
    {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *playerDictionary=(CCDictionary*)(directory->objectForKey(key->m_sString));
        SLevelPlayer *playerLevel=new SLevelPlayer;
        playerLevel->m_iLevel=GameTools::intForKey("level", playerDictionary);
        playerLevel->m_iCard_max=GameTools::intForKey("card_max", playerDictionary);
        playerLevel->m_iLeader_max=GameTools::intForKey("leader_max", playerDictionary);
        playerLevel->m_iFriend_max=GameTools::intForKey("friend_max",playerDictionary); //好友上线
        playerLevel->m_iHP_max=GameTools::intForKey("hp_max",playerDictionary);
        playerLevel->m_iMp_max=GameTools::intForKey("mp_max",playerDictionary);
        playerLevel->m_iExp_max=GameTools::intForKey("exp_max", playerDictionary);
        vPlayerLevel->push_back(playerLevel);
    }
    if(vPlayerLevel->size()>0)
    {
        return true;
    }
    return false;
}

bool CConfigResourceLoad::loadSkillLogicInfo(vector<CSkillData *> &vSkillTable,const char *fileName)
{
    
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(fileName);
    if(directory==NULL)
    {
        return false;
    }
    CCArray *vKeyArray=directory->allKeys();
    if(vKeyArray->count()==0)
    {
        return false;
    }
    for (int i=0; i<vKeyArray->count(); i++)
    {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *skillDirector=(CCDictionary*)(directory->objectForKey(key->m_sString));
        CSkillData *skill=new CSkillData;
        skill->skill_id=GameTools::intForKey("skill_id", skillDirector);
        skill->skill_type=GameTools::intForKey("skill_type", skillDirector);
        skill->skill_logic=GameTools::intForKey("skill_logic", skillDirector);
        skill->cost_func_1=GameTools::intForKey("cost_func_1", skillDirector);
        skill->cost_parameter_1=GameTools::intForKey("cost_parameter_1", skillDirector);
        skill->cost_parameter_p_1=GameTools::intForKey("cost_parameter_p_1", skillDirector);
        skill->cost_func_2=GameTools::intForKey("cost_func_2", skillDirector);
        skill->cost_parameter_2=GameTools::intForKey("cost_parameter_2", skillDirector);
        skill->cost_parameter_p_2=GameTools::intForKey("cost_parameter_p_2", skillDirector);
        skill->parameter_1=GameTools::intForKey("parameter_1", skillDirector);
        skill->parameter_2=GameTools::intForKey("parameter_2", skillDirector);
        skill->parameter_3=GameTools::intForKey("parameter_3", skillDirector);
        skill->parameter_4=GameTools::intForKey("parameter_4", skillDirector);
        skill->parameter_5=GameTools::intForKey("parameter_5", skillDirector);
        skill->parameter_6=GameTools::intForKey("parameter_6", skillDirector);
        skill->effect_plist=GameTools::valueForKey("effect_plist", skillDirector);
        skill->m_skillName=GameTools::valueForKey("skill_name", skillDirector);
        skill->m_skillTrip=GameTools::valueForKey("skill_tips", skillDirector);

        vSkillTable.push_back(skill);
    }
    return true;
}

bool CConfigResourceLoad::loadEffectLogicInfo(vector<CImapact *> &vImapactTable,const char *fileName)
{
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(fileName);
    if(directory==NULL)
    {
        return false;
    }
    CCArray *vKeyArray=directory->allKeys();
    if(vKeyArray->count()==0)
    {
        return false;
    }
    for (int i=0; i<vKeyArray->count(); i++)
    {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *effectDirector=(CCDictionary*)(directory->objectForKey(key->m_sString));
        CImapact *skillEffect=new CImapact;
        skillEffect->m_ieffect_id=GameTools::intForKey("effect_id", effectDirector);
        skillEffect->m_ieffect_logic=GameTools::intForKey("effect_logic", effectDirector);
        skillEffect->m_ifdelay=GameTools::intForKey("if_delay", effectDirector);
        skillEffect->m_irandom=GameTools::intForKey("if_radom", effectDirector);
        skillEffect->m_process=GameTools::intForKey("process", effectDirector);
        skillEffect->m_ibuff= GameTools::intForKey("effect_time", effectDirector);
        skillEffect->m_ishowtime=GameTools::intForKey("keep_time", effectDirector);
        skillEffect->m_iMutex=GameTools::intForKey("mutex", effectDirector);
        skillEffect->m_iMutex_level=GameTools::intForKey("mutex_level", effectDirector);
        skillEffect->m_iParameter_1=GameTools::intForKey("parameter_1", effectDirector);
        skillEffect->m_iParameter_2=GameTools::intForKey("parameter_2", effectDirector);
        skillEffect->m_iParameter_3=GameTools::intForKey("parameter_3", effectDirector);
        skillEffect->m_iParameter_4=GameTools::intForKey("parameter_4", effectDirector);
        skillEffect->m_iParameter_5=GameTools::intForKey("parameter_5", effectDirector);
        skillEffect->m_iParameter_6=GameTools::intForKey("parameter_6", effectDirector);
        skillEffect->m_iParameter_7=GameTools::intForKey("parameter_7", effectDirector);
        skillEffect->m_iParameter_8=GameTools::intForKey("parameter_8", effectDirector);
        skillEffect->m_iParameter_9=GameTools::intForKey("parameter_9", effectDirector);
        skillEffect->m_iParameter_10=GameTools::intForKey("parameter_10", effectDirector);
        skillEffect->m_sEffectFile=GameTools::valueForKey("effect_plistfile", effectDirector);
        vImapactTable.push_back(skillEffect);
    }
    return true;
}