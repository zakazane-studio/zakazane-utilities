#include "Zakazane/ZkzNameBPFunctionLibrary.h"

FName UZkzNameBPFunctionLibrary::MakeNameNone()
{
	return NAME_None;
}

bool UZkzNameBPFunctionLibrary::IsNameNone(FName Name)
{
	return Name == NAME_None;
}
