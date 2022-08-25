set_project("TiebaAutoSign")

set_version("0.0.2")

add_requires("openssl")

add_rules("mode.debug", "mode.release")

set_toolchains("clang")
set_languages("c++17")

add_includedirs("include")

target("main")
    set_default(true)
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("openssl")
    set_rundir("$(projectdir)")