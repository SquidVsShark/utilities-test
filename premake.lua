solution "Utilities Test"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "Unit"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "**.c", "**.hpp", "**.cpp", "../utilities/**.cpp", "../utilities/**.hpp"}
      includedirs { "../utilities/", "./" }
      
      buildoptions {
        "-std=c++14"
      }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }   
