##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=C-Philosophenproblem
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/michi/OneDrive/BSgit/Bsp2
ProjectPath            :=/home/michi/OneDrive/BSgit/Bsp2/C-Philosophenproblem
IntermediateDirectory  :=../build-$(ConfigurationName)/C-Philosophenproblem
OutDir                 :=../build-$(ConfigurationName)/C-Philosophenproblem
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=michi
Date                   :=19/11/19
CodeLitePath           :=/home/michi/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../build-$(ConfigurationName)/bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  -pthread
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(ObjectSuffix) ../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(ObjectSuffix) ../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(ObjectSuffix) ../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(ObjectSuffix) ../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/C-Philosophenproblem/.d $(Objects) 
	@mkdir -p "../build-$(ConfigurationName)/C-Philosophenproblem"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "../build-$(ConfigurationName)/C-Philosophenproblem"
	@mkdir -p ""../build-$(ConfigurationName)/bin""

../build-$(ConfigurationName)/C-Philosophenproblem/.d:
	@mkdir -p "../build-$(ConfigurationName)/C-Philosophenproblem"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(ObjectSuffix): main.c ../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/C-Philosophenproblem/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(DependSuffix) -MM main.c

../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/C-Philosophenproblem/main.c$(PreprocessSuffix) main.c

../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(ObjectSuffix): gewichte.c ../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/C-Philosophenproblem/gewichte.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gewichte.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(DependSuffix): gewichte.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(DependSuffix) -MM gewichte.c

../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(PreprocessSuffix): gewichte.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/C-Philosophenproblem/gewichte.c$(PreprocessSuffix) gewichte.c

../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(ObjectSuffix): err.c ../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/C-Philosophenproblem/err.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/err.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(DependSuffix): err.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(DependSuffix) -MM err.c

../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(PreprocessSuffix): err.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/C-Philosophenproblem/err.c$(PreprocessSuffix) err.c

../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(ObjectSuffix): routine.c ../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/C-Philosophenproblem/routine.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/routine.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(DependSuffix): routine.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(DependSuffix) -MM routine.c

../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(PreprocessSuffix): routine.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/C-Philosophenproblem/routine.c$(PreprocessSuffix) routine.c

../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(ObjectSuffix): controller.c ../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/C-Philosophenproblem/controller.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/controller.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(DependSuffix): controller.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(DependSuffix) -MM controller.c

../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(PreprocessSuffix): controller.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/C-Philosophenproblem/controller.c$(PreprocessSuffix) controller.c


-include ../build-$(ConfigurationName)/C-Philosophenproblem//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


