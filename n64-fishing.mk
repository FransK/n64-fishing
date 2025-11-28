build/code/n64-fishing/%.o: N64_CXXFLAGS += -std=gnu++20 -fno-exceptions -O2

N64_FISHING_assets_coll = $(wildcard assets/n64-fishing/*.coll)
N64_FISHING_assets_scene = $(wildcard assets/n64-fishing/*.scene)

N64_FISHING_assets_png = $(wildcard assets/n64-fishing/*.png) $(wildcard assets/n64-fishing/grass/*.png) \
	$(wildcard assets/n64-fishing/ui/*.png) $(wildcard assets/n64-fishing/ptx/*.png) \
	$(wildcard assets/n64-fishing/obj/*.png)

N64_FISHING_assets_glb = $(wildcard assets/n64-fishing/*.glb) $(wildcard assets/n64-fishing/obj/*.glb)
N64_FISHING_assets_ttf = $(wildcard assets/n64-fishing/*.ttf)
N64_FISHING_assets_mp3 = $(wildcard assets/n64-fishing/bgm/*.mp3)
N64_FISHING_assets_wav = $(wildcard assets/n64-fishing/sfx/*.wav)

N64_FISHING_assets_conv = $(patsubst assets/%,filesystem/%,$(N64_FISHING_assets_coll)) \
              			 $(patsubst assets/%,filesystem/%,$(N64_FISHING_assets_scene)) \
              			 $(patsubst assets/%,filesystem/%,$(N64_FISHING_assets_png:%.png=%.sprite)) \
              			 $(patsubst assets/%,filesystem/%,$(N64_FISHING_assets_glb:%.glb=%.t3dm)) \
              			 $(patsubst assets/%,filesystem/%,$(N64_FISHING_assets_ttf:%.ttf=%.font64)) \
              			 $(patsubst assets/%,filesystem/%,$(N64_FISHING_assets_mp3:%.mp3=%.wav64)) \
              			 $(patsubst assets/%,filesystem/%,$(N64_FISHING_assets_wav:%.wav=%.wav64))

#assets/n64-fishing/map.scene: assets/n64-fishing/map.glb
#	@echo "    [SCENE] $@"
#	code/n64-fishing/tools/gltf_to_scene "$<" assets/n64-fishing/map.scene

#assets/n64-fishing/map.coll: assets/n64-fishing/map.glb
#	@echo "    [COLL] $@"
#	code/n64-fishing/tools/gltf_to_coll "$<" assets/n64-fishing/map.coll

filesystem/n64-fishing/%.coll: assets/n64-fishing/%.coll
	@mkdir -p $(dir $@)
	@echo "    [COLL] $@"
	$(N64_BINDIR)/mkasset -c 3 -w 256 -o filesystem/n64-fishing "$<"

filesystem/n64-fishing/%.scene: assets/n64-fishing/%.scene
	@mkdir -p $(dir $@)
	@echo "    [SCENE] $@"
	$(N64_BINDIR)/mkasset -c 2 -w 256 -o filesystem/n64-fishing "$<"

N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 22050 --wav-mono

filesystem/n64-fishing/bgm/%.wav64: assets/n64-fishing/bgm/%.mp3
	@mkdir -p $(dir $@)
	@echo "    [SFX] $@"
	$(N64_AUDIOCONV) $(N64_FISHING_AUDIOCONV_FLAGS) -o $(dir $@) "$<"

filesystem/n64-fishing/sfx/%.wav64: assets/n64-fishing/sfx/%.wav
	@mkdir -p $(dir $@)
	@echo "    [SFX] $@"
	$(N64_AUDIOCONV) $(N64_FISHING_AUDIOCONV_FLAGS) --wav-compress 0 -o $(dir $@) "$<"
	# $(N64_BINDIR)/mkasset -c 3 -o $(dir $@) $@


# 8000, 16000, 22050, 32000, 44100
filesystem/n64-fishing/sfx/Coin.wav64: N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 32000 --wav-mono
filesystem/n64-fishing/sfx/Men0.wav64: N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 32000 --wav-mono

filesystem/n64-fishing/sfx/PotBreak.wav64: N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 16000 --wav-mono
filesystem/n64-fishing/sfx/VoidOn.wav64: N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 18000 --wav-mono
filesystem/n64-fishing/sfx/BoxHit.wav64: N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 18000 --wav-mono

filesystem/n64-fishing/bgm/Main.wav64: N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 24000 --wav-mono --wav-compress 1
filesystem/n64-fishing/bgm/Boss.wav64: N64_FISHING_AUDIOCONV_FLAGS = --wav-resample 19000 --wav-mono --wav-compress 1
filesystem/n64-fishing/map.t3dm: T3DM_FLAGS += --bvh

ASSETS_LIST += \
	filesystem/n64-fishing/player.t3dm