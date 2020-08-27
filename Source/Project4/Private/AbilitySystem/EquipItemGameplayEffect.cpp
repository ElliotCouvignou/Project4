// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/EquipItemGameplayEffect.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

UEquipItemGameplayEffect::UEquipItemGameplayEffect(UDataTable* DataTable)
{
	SetupModifiersFromItemDataTable(DataTable);
	//SetupGrantedTagsEffectArmorType(Armortype);
}

UEquipItemGameplayEffect::UEquipItemGameplayEffect(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEquipItemGameplayEffect::SetupModifiersFromItemDataTable(UDataTable* DataTable)
{
	static const FString Context = FString(TEXT("UAttribute::BindToMetaDataTable"));

	for (TFieldIterator<FProperty> It(GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		FProperty* Property = *It;
		FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property);
		/*if (NumericProperty)
		{
			FString RowNameStr = FString::Printf(TEXT("%s.%s"), *Property->GetOwnerVariant().GetName(), *Property->GetName());

			FAttributeMetaData* MetaData = DataTable->FindRow<FAttributeMetaData>(FName(*RowNameStr), Context, false);
			if (MetaData)
			{
				FGameplayModifierInfo Modifier;
				void* Data = NumericProperty->ContainerPtrToValuePtr<void>(this);
				NumericProperty->SetFloatingPointPropertyValue(Data, MetaData->BaseValue);
			}
		}
		else*/ if (FGameplayAttribute::IsGameplayAttributeDataProperty(Property))
		{
			FString RowNameStr = FString::Printf(TEXT("%s.%s"), *Property->GetOwnerVariant().GetName(), *Property->GetName());

			FAttributeMetaData* MetaData = DataTable->FindRow<FAttributeMetaData>(FName(*RowNameStr), Context, false);
			if (MetaData)
			{
				FGameplayAttribute Attribute = FGameplayAttribute(Property);
				print(FString("Attribute Name: " + Attribute.GetName()));

				FStructProperty* StructProperty = CastField<FStructProperty>(Property);
				check(StructProperty);
				FGameplayAttributeData* DataPtr = StructProperty->ContainerPtrToValuePtr<FGameplayAttributeData>(this);
				check(DataPtr);
				DataPtr->SetBaseValue(MetaData->BaseValue);
				DataPtr->SetCurrentValue(MetaData->BaseValue);
			}
		}
	}
}


void UEquipItemGameplayEffect::SetupGrantedTagsEffectArmorType(EArmorType Armortype)
{
}
