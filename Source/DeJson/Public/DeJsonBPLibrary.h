#pragma once

#include "JsonObjectWrapper.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DeJsonBPLibrary.generated.h"

class FJsonValue;

/*******************
 * JSON Field Struct
 *******************/
USTRUCT(BlueprintType)
struct FDeJsonField {
	GENERATED_BODY()

	// TSharedPtr can't have UPROPERTY
	TSharedPtr<FJsonValue> JsonValue;
};

/************************
 * DeJson BP Function lib
 ************************/
UCLASS()
class UDeJsonBPLibrary : public UBlueprintFunctionLibrary {
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="DeJson|Convert", CustomThunk, meta = (CustomStructureParam = "Value"))
	static bool ConvertJsonStringToStruct(
		const FString& JsonString, int32& Value
	);

	DECLARE_FUNCTION(execConvertJsonStringToStruct);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="DeJson|Convert", CustomThunk, meta = (CustomStructureParam = "Value", CompactNodeTitle="DeJsonField"))
	static void ConvertToDeJsonField(const int32& Value, FDeJsonField& DeJsonField);

	DECLARE_FUNCTION(execConvertToDeJsonField);

	UFUNCTION(BlueprintCallable, Category="DeJson|Create")
	static void CreateJsonObject(const TMap<FString, FDeJsonField>& JsonFields, FJsonObjectWrapper& JsonObject);

	UFUNCTION(BlueprintCallable, Category="DeJson|Create")
	static void CreateJsonObjectString(const TMap<FString, FDeJsonField>& JsonFields, FString& JsonString);
	
	UFUNCTION(BlueprintPure, Category = "DeJson|Struct")
	static int32 GetStructMemberCount(const UScriptStruct* Struct);

	UFUNCTION(BlueprintPure, Category = "DeJson|Struct")
	static TArray<FString> GetStructMemberNames(const UScriptStruct* Struct);
};
