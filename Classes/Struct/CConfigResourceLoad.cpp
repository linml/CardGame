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
        card->m_ileve_max=GameTools::intForKey("card_leve_max", cardDirector);   //等级最高级别
        card->m_icard_attack=GameTools::intForKey("card_attack", cardDirector); //攻击力
        card->m_icard_defend=GameTools::intForKey("card_defend", cardDirector); //防御力
        card->m_icardhp=GameTools::intForKey("card_hp",cardDirector);// 卡牌的总的HP
        card->m_iusually_attack=GameTools::intForKey("usually_attack", cardDirector);
        card->m_iskillLine=GameTools::intForKey("skill_anger", cardDirector);
        card->m_iskillHelp=GameTools::intForKey("skill_help", cardDirector);
        card->m_iskillDead=GameTools::intForKey("skill_dead", cardDirector);
        card->m_iskillBuff=GameTools::intForKey("skill_buff", cardDirector);
        card->m_scard_tips=string(GameTools::valueForKey("card_tips", cardDirector));
        card->m_scard_resources=GameTools::valueForKey("card_resources", cardDirector);
        card->m_scard_head=GameTools::valueForKey("card_head", cardDirector);
        card->m_scard_groud=GameTools::valueForKey("card_groud", cardDirector);
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
        playerLevel->m_iLeadership=GameTools::intForKey("leadership", playerDictionary);
        playerLevel->m_iFriend_max=GameTools::intForKey("friend_max",playerDictionary); //好友上线
        playerLevel->m_iHP=GameTools::intForKey("hp",playerDictionary);
        playerLevel->m_iMp=GameTools::intForKey("mp",playerDictionary);
        playerLevel->m_iexp=GameTools::intForKey("exp", playerDictionary);
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
        skill->stskill_id=GameTools::intForKey("stskill_id", skillDirector);
        skill->cost_1_func=GameTools::intForKey("cost_1_func", skillDirector);
        skill->cost_1_parameter1=GameTools::intForKey("cost_1_parameter1", skillDirector);
        skill->cost_1_parameter2=GameTools::intForKey("cost_1_parameter2", skillDirector);
        skill->cost_2_func=GameTools::intForKey("cost_2_func", skillDirector);
        skill->cost_2_parameter1=GameTools::intForKey("cost_2_parameter1", skillDirector);
        skill->cost_2_parameter2=GameTools::intForKey("cost_2_parameter2", skillDirector);
        skill->parameter_1=GameTools::intForKey("parameter_1", skillDirector);
        skill->parameter_2=GameTools::intForKey("parameter_2", skillDirector);
        skill->parameter_3=GameTools::intForKey("parameter_3", skillDirector);
        skill->parameter_4=GameTools::intForKey("parameter_4", skillDirector);
        skill->effect_plist=GameTools::valueForKey("effect_plist", skillDirector);

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
        skillEffect->m_iImapactid=GameTools::intForKey("m_iImapactid", effectDirector);
        skillEffect->m_iImpact_name=GameTools::intForKey("m_iImpact_name", effectDirector);
        skillEffect->m_iStimpact_id=GameTools::intForKey("m_iStimpact_id", effectDirector);
        skillEffect->m_bIsBuff=GameTools::intForKey("m_bIsBuff", effectDirector);
        skillEffect->m_iMutex=GameTools::intForKey("m_iMutex", effectDirector);
        skillEffect->m_iMutex_level=GameTools::intForKey("m_iMutex_level", effectDirector);
        skillEffect->m_iParameter_1=GameTools::intForKey("m_iParameter_1", effectDirector);
        skillEffect->m_iParameter_2=GameTools::intForKey("m_iParameter_2", effectDirector);
        skillEffect->m_iParameter_3=GameTools::intForKey("m_iParameter_3", effectDirector);
        skillEffect->m_iParameter_4=GameTools::intForKey("m_iParameter_4", effectDirector);
        skillEffect->m_iParameter_5=GameTools::intForKey("m_iParameter_5", effectDirector);
        skillEffect->m_iParameter_6=GameTools::intForKey("m_iParameter_6", effectDirector);
        skillEffect->m_iParameter_7=GameTools::intForKey("m_iParameter_7", effectDirector);
        skillEffect->m_iParameter_8=GameTools::intForKey("m_iParameter_8", effectDirector);
         skillEffect->m_iParameter_9=GameTools::intForKey("m_iParameter_9", effectDirector);
         skillEffect->m_iParameter_10=GameTools::intForKey("m_iParameter_10", effectDirector);
        vImapactTable.push_back(skillEffect);
    }
    return true;
}