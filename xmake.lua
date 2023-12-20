set_languages("cxx20")
add_rules("mode.debug", "mode.release")

target("Reversi")
    set_kind("binary")
    add_files("src/*.cpp")
