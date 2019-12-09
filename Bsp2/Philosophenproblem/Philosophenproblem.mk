##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Philosophenproblem
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/michi/OneDrive/BSgit/Bsp2
ProjectPath            :=/home/michi/OneDrive/BSgit/Bsp2/Philosophenproblem
IntermediateDirectory  :=../build-$(ConfigurationName)/Philosophenproblem
OutDir                 :=../build-$(ConfigurationName)/Philosophenproblem
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=michi
Date                   :=17/11/19
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
Objects0=../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/Philosophenproblem/.d $(Objects) 
	@mkdir -p "../build-$(ConfigurationName)/Philosophenproblem"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "../build-$(ConfigurationName)/Philosophenproblem"
	@mkdir -p ""../build-$(ConfigurationName)/bin""

../build-$(ConfigurationName)/Philosophenproblem/.d:
	@mkdir -p "../build-$(ConfigurationName)/Philosophenproblem"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(ObjectSuffix): main.cpp ../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/Philosophenproblem/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(DependSuffix) -MM main.cpp

../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/Philosophenproblem/main.cpp$(PreprocessSuffix) main.cpp

../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(ObjectSuffix): philothread.cpp ../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/Philosophenproblem/philothread.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/philothread.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(DependSuffix): philothread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(DependSuffix) -MM philothread.cpp

../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(PreprocessSuffix): philothread.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/Philosophenproblem/philothread.cpp$(PreprocessSuffix) philothread.cpp

../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(ObjectSuffix): gewichte.cpp ../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/michi/OneDrive/BSgit/Bsp2/Philosophenproblem/gewichte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gewichte.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(DependSuffix): gewichte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(DependSuffix) -MM gewichte.cpp

../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(PreprocessSuffix): gewichte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/Philosophenproblem/gewichte.cpp$(PreprocessSuffix) gewichte.cpp


-include ../build-$(ConfigurationName)/Philosophenproblem//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


