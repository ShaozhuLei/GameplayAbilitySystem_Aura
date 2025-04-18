// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadScreenSaveGame.h"

//找出结构体中谁的MapName是形参
FSavedMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap& Map: SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
			return Map;
	} 
	return FSavedMap();
	
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap& Map: SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
			return true;
	} 
	return false;
}
