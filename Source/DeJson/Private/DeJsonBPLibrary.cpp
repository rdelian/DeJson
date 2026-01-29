#include "DeJsonBPLibrary.h"

#include "JsonDomBuilder.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"
#include "Blueprint/BlueprintExceptionInfo.h"


UDeJsonBPLibrary::UDeJsonBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}


void UDeJsonBPLibrary::CreateJsonObject(const TMap<FString, FDeJsonField>& JsonFields, FJsonObjectWrapper& JsonObject) {
	for (const auto JsonField : JsonFields) {
		JsonObject.JsonObject->SetField(JsonField.Key, JsonField.Value.JsonValue);
	}
}


void UDeJsonBPLibrary::CreateJsonObjectString(const TMap<FString, FDeJsonField>& JsonFields, FString& JsonString) {
	FJsonObjectWrapper JsonObject;

	CreateJsonObject(JsonFields, JsonObject);

	JsonObject.JsonObjectToString(JsonString);
}


int32 UDeJsonBPLibrary::GetStructMemberCount(const UScriptStruct* Struct) {
	if (Struct == nullptr) return 0;

	int32 Count = 0;

	for (TFieldIterator<FProperty> It(Struct); It; ++It) {
		Count++;
	}

	return Count;
}


TArray<FString> UDeJsonBPLibrary::GetStructMemberNames(const UScriptStruct* Struct) {
	if (Struct == nullptr) return {};

	TArray<FString> FieldNames;

	for (TFieldIterator<FProperty> It(Struct); It; ++It) {
		if (const FProperty* Property = *It) {
			FieldNames.Add(Property->GetAuthoredName());
		}
	}

	return FieldNames;
}


DEFINE_FUNCTION(UDeJsonBPLibrary::execConvertJsonStringToStruct) {
	P_GET_PROPERTY(FStrProperty, JsonString);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);

	void* ValuePtr = Stack.MostRecentPropertyAddress;
	FProperty* ValueProp = Stack.MostRecentProperty;

	P_FINISH;

	bool bSuccess = false;

	if (!ValueProp || !ValuePtr) {
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("ConvertJsonStringToStruct", "Value_MissingInputProperty", "Failed to resolve the input parameter for Value.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}

	const FStructProperty* StructProperty = CastField<FStructProperty>(ValueProp);
	if (!StructProperty || StructProperty->Struct == nullptr) {
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("ConvertJsonStringToStruct", "Value_BadTypeh", "Value is not a struct.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}

	P_NATIVE_BEGIN;
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid()) {
			bSuccess = FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), StructProperty->Struct, ValuePtr);
		}
	P_NATIVE_END;

	*StaticCast<bool*>(RESULT_PARAM) = bSuccess;
}


DEFINE_FUNCTION(UDeJsonBPLibrary::execConvertToDeJsonField) {
	// Value (wildcard)
	Stack.StepCompiledIn<FProperty>(nullptr);
	FProperty* ValueProp = Stack.MostRecentProperty;
	const void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_GET_STRUCT_REF(FDeJsonField, OutDeJsonField);

	P_FINISH;

	if (!ValueProp || !ValuePtr) {
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("ConvertToDeJsonField", "Value_MissingInputProperty", "Failed to resolve the input parameter for Value.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}

	P_NATIVE_BEGIN;
		OutDeJsonField.JsonValue = FJsonObjectConverter::UPropertyToJsonValue(ValueProp, ValuePtr);
	P_NATIVE_END;
}
