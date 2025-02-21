// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass) const
{
	//通过角色类别 Key返回对应的结构体
	return CharacterClassInformation.FindChecked(CharacterClass);
}
