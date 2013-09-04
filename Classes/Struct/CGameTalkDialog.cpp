//
//  CGameTalkDialog.cpp
//  91.cube
//
//  Created by linminglu on 13-9-4.
//
//

#include "CGameTalkDialog.h"
CGameTalkDialog::CGameTalkDialog()
{
    m_iGameTalkID=0;
    m_iGameTalkUiPlan=1;
}

CGameTalkDialog::~CGameTalkDialog()
{
    
}

bool CGameTalkDialog::operator==(const CGameTalkDialog &orig)
{
    return ((this->m_iGameTalkID==orig.m_iGameTalkID) ||
            (this->m_iGameTalkUiPlan==this->m_iGameTalkUiPlan && this->m_sGameTalkDialogPng ==this->m_sGameTalkDialogPng));
}