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
#include "CGameNpcCard.h"
#include "CStructShopInfo.h"
#include "CStructShopSellItem.h"

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
        card->m_icard_energyMax=GameTools::intForKey("card_energy", cardDirector);
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
        //CCLog("the %i card's next card id: %d", card->m_icard_id, card->m_ccard_next);
        // change end:
        if(hashmapAllCard[key->intValue()])
        {
            delete hashmapAllCard[key->intValue()];
            hashmapAllCard[key->intValue()]=NULL;
         }
        hashmapAllCard[key->intValue()]=card;

    }
    cout<<"card plist size ="<<hashmapAllCard.size()<<endl;
    if(hashmapAllCard.size()>0)
    {
        return true;
    }
    return false;
    
    
}

bool CConfigResourceLoad::loadNPCCardInfo(map<int, CCard *> &hashMapNpcCardAll, const char *fileName)
{
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(fileName);
    CCArray *vKeyArray=directory->allKeys();
    if(!vKeyArray)
    {
        return false;
    }
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *cardDirector=(CCDictionary*)(directory->objectForKey(key->m_sString));
        CCard *card=new CNpcCard();
        card->m_icard_id=GameTools::intForKey("npc_id", cardDirector);
        card->m_scard_name=string(GameTools::valueForKey("name", cardDirector));
        card->m_ccard_next=GameTools::intForKey("card_next", cardDirector);  ///背景底色
        card->m_sicard_star=GameTools::intForKey("star", cardDirector);
        card->m_icard_stirps=GameTools::intForKey("stirps", cardDirector);   //种族
        card->m_icard_suit=GameTools::intForKey("suit", cardDirector);      //随机数值
        card->m_icard_attack=GameTools::intForKey("attack", cardDirector); //攻击力
        card->m_icard_defend=GameTools::intForKey("defend", cardDirector); //防御力
        card->m_icardhp=GameTools::intForKey("hp",cardDirector);// 卡牌的总的HP
        card->m_iusually_attack=GameTools::intForKey("usually_attack", cardDirector);
        card->m_icard_energyMax=GameTools::intForKey("card_energy", cardDirector);
        card->m_iskillLine=GameTools::intForKey("skill_energy", cardDirector);
        card->m_iskillHelp=GameTools::intForKey("skill_help", cardDirector);
        card->m_iskillDead=GameTools::intForKey("skill_dead", cardDirector);
        card->m_iskillBuff=GameTools::intForKey("skill_buff", cardDirector);
        card->m_scard_tips=GameTools::valueForKey("card_tips", cardDirector);
        card->m_scard_resources=GameTools::valueForKey("card_resources", cardDirector);
        card->m_scard_head=GameTools::valueForKey("card_head", cardDirector);
        card->m_scard_ground=GameTools::valueForKey("card_ground", cardDirector);
        card->m_scard_role="card_role_"+card->m_scard_resources;
        card->m_scard_resources="peo"+card->m_scard_resources+".png";
        ((CNpcCard *)card)->m_npcCardLevel=GameTools::intForKey("level",cardDirector);
        int index = card->m_sicard_star -1;
        index = abs(index) > 7 ? 7: abs(index);
        card->m_ileve_max = g_aMaxLevel[index];
        if(hashMapNpcCardAll[key->intValue()])
        {
            delete  hashMapNpcCardAll[key->intValue()];
            hashMapNpcCardAll[key->intValue()]=NULL;
        }
        hashMapNpcCardAll[key->intValue()]=card;
    }
    cout<<"card plist size ="<<hashMapNpcCardAll.size()<<endl;
    if(hashMapNpcCardAll.size()>0)
    {
        return true;
    }
    return false;
}




bool CConfigResourceLoad::loadPlayerLevelInfo(map<int ,SLevelPlayer *> &vPlayerLevel, const char *playerFileName)
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
        playerLevel->m_iGp_max=GameTools::intForKey("gp_max",playerDictionary);
        playerLevel->m_iAp_max=GameTools::intForKey("ap_max",playerDictionary);
        playerLevel->m_iExp_max=GameTools::intForKey("exp_max", playerDictionary);
        if ( vPlayerLevel[playerLevel->m_iLevel])
        {
            delete   vPlayerLevel[playerLevel->m_iLevel];
            vPlayerLevel[playerLevel->m_iLevel]=NULL;
        }
        vPlayerLevel[playerLevel->m_iLevel]=playerLevel;
    }
    if(vPlayerLevel.size()>0)
    {
        return true;
    }
    return false;
}

bool CConfigResourceLoad::loadSkillLogicInfo(map<int,CSkillData *> &vSkillTable,const char *fileName)
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
        if (vSkillTable[skill->skill_id]) {
            delete vSkillTable[skill->skill_id];
            vSkillTable[skill->skill_id]=NULL;
        }
        vSkillTable[skill->skill_id]=skill;
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
        skillEffect->m_ifdelay=GameTools::intForKey("delay", effectDirector);
        skillEffect->m_irandom=GameTools::intForKey("random", effectDirector);
        skillEffect->m_process=GameTools::intForKey("backtrack", effectDirector);
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

bool CConfigResourceLoad::loadShopSellItem(CStructShopInfo *shopInfo,const char *fileName)
{
    CCDictionary *directory = CCDictionary::createWithContentsOfFile(fileName);
    if(directory)
    {
        CCArray *vKeyArray=directory->allKeys();
        if(vKeyArray->count()!=0)
        {
            for (int i=0; i<vKeyArray->count(); i++)
            {
                CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
                if(key->intValue()==1)
                {
                    CCDictionary *shopInfoDict=(CCDictionary*)(directory->objectForKey(key->m_sString));
                    shopInfo->setShopId(key->intValue());
                    shopInfo->setShopName(GameTools::valueForKey("shop_name", shopInfoDict));
                    shopInfo->setShopType(GameTools::intForKey("type", shopInfoDict));
                    shopInfo->setShopRondomNumber(GameTools::intForKey("rondom_num", shopInfoDict));
                    shopInfo->setShopIsRondom(GameTools::boolForKey("is_rondom",shopInfoDict));
                    shopInfo->setShopReloadTime(GameTools::intForKey("reload_num", shopInfoDict));
                    for (int i=1; i<20; i++)
                    {
                        //解析20个商品的东西 我了个草。。。
                        char getParam[20];
                        sprintf(getParam, "item_id_%d",i);
                        int itemKeyId=GameTools::intForKey(getParam, shopInfoDict);
                        if(itemKeyId!=0)
                        {
                            
                            sprintf(getParam, "item_num_%d",i);
                            CStructShopSellItem *item=new CStructShopSellItem(itemKeyId);
                            item->setGroupNum(GameTools::intForKey(getParam, shopInfoDict));
                            sprintf(getParam, "limit_num_%d",i);

                            int limitNumber=GameTools::intForKey(getParam, shopInfoDict);
                            unsigned int maxNumber= (limitNumber==0 ? UINT_MAX:limitNumber);
                            item->setItemSellMaxNum(maxNumber);
                            sprintf(getParam, "price_%d",i);
                            int oldValue=GameTools::intForKey(getParam, shopInfoDict) ;
                            sprintf(getParam, "percent_%d",i);
                            int price=(int)(oldValue* (GameTools::intForKey(getParam, shopInfoDict))*0.01);
                            item->setOldValue(oldValue);
                            item->setValue(price);
//                            if (shopInfo->mapShopItem[itemKeyId]) {
//                                CCLog("商品表里面的商品不能有重复的 策划有错误");
//                                delete  shopInfo->mapShopItem[itemKeyId];
//                                shopInfo->mapShopItem[itemKeyId]=NULL;
//                            }
                            shopInfo->mapShopItem.push_back(item);
                        }
                    }
                   
                }
            }
            return true;
        }
    }
    CCLog("商店配置表格有错误");
    return false;

}