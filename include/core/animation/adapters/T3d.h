#pragma once

#include <libdragon.h>
#include <t3d/t3danim.h>
#include <memory>
#include <string>

namespace Adapters
{
    class SpriteAdapter
    {
    public:
        SpriteAdapter() = default;
        explicit SpriteAdapter(const char *path)
            : mSprite(sprite_load(path)) {}

        SpriteAdapter(const SpriteAdapter &) = delete;
        SpriteAdapter &operator=(const SpriteAdapter &) = delete;
        SpriteAdapter(SpriteAdapter &&other) noexcept : mSprite(std::move(other.mSprite)) {}
        SpriteAdapter &operator=(SpriteAdapter &&other) noexcept
        {
            mSprite = std::move(other.mSprite);
            return *this;
        }

        sprite_t *get() const noexcept { return mSprite.get(); }
        explicit operator bool() const noexcept { return mSprite != nullptr; }

    private:
        struct Deleter
        {
            void operator()(sprite_t *p) const noexcept
            {
                if (p)
                    sprite_free(p);
            }
        };

        std::unique_ptr<sprite_t, Deleter> mSprite;
    };

    class ModelAdapter
    {
    public:
        explicit ModelAdapter(std::string path)
            : mModel(t3d_model_load(path.c_str())) {}

        ModelAdapter(const ModelAdapter &) = delete;
        ModelAdapter &operator=(const ModelAdapter &) = delete;
        ModelAdapter(ModelAdapter &&other) noexcept : mModel(std::move(other.mModel)) {}
        ModelAdapter &operator=(ModelAdapter &&other) noexcept
        {
            mModel = std::move(other.mModel);
            return *this;
        }

        T3DModel *getModel() const noexcept { return mModel.get(); }
        explicit operator bool() const noexcept { return mModel != nullptr; }

    private:
        struct Deleter
        {
            void operator()(T3DModel *p) const noexcept
            {
                if (p)
                    t3d_model_free(p);
            }
        };

        std::unique_ptr<T3DModel, Deleter> mModel;
    };

    class Mat4FPAdapter
    {
    public:
        Mat4FPAdapter()
            : mMat((T3DMat4FP *)malloc_uncached(sizeof(T3DMat4FP))) {}

        Mat4FPAdapter(const Mat4FPAdapter &) = delete;
        Mat4FPAdapter &operator=(const Mat4FPAdapter &) = delete;
        Mat4FPAdapter(Mat4FPAdapter &&other) noexcept : mMat(std::move(other.mMat)) {}
        Mat4FPAdapter &operator=(Mat4FPAdapter &&other) noexcept
        {
            mMat = std::move(other.mMat);
            return *this;
        }

        T3DMat4FP *get() const noexcept { return mMat.get(); }
        explicit operator bool() const noexcept { return mMat != nullptr; }

    private:
        struct Deleter
        {
            void operator()(T3DMat4FP *p) const noexcept
            {
                if (p)
                    free_uncached(p);
            }
        };

        std::unique_ptr<T3DMat4FP, Deleter> mMat;
    };

    class RspqBlockAdapter
    {
    public:
        RspqBlockAdapter() = default;
        explicit RspqBlockAdapter(rspq_block_t *block) : mBlock(block) {}

        RspqBlockAdapter(const RspqBlockAdapter &) = delete;
        RspqBlockAdapter &operator=(const RspqBlockAdapter &) = delete;
        RspqBlockAdapter(RspqBlockAdapter &&other) noexcept : mBlock(std::move(other.mBlock)) {}
        RspqBlockAdapter &operator=(RspqBlockAdapter &&other) noexcept
        {
            mBlock = std::move(other.mBlock);
            return *this;
        }

        rspq_block_t *get() const noexcept { return mBlock.get(); }
        explicit operator bool() const noexcept { return mBlock != nullptr; }

    private:
        struct Deleter
        {
            void operator()(rspq_block_t *p) const noexcept
            {
                if (p)
                    rspq_block_free(p);
            }
        };

        std::unique_ptr<rspq_block_t, Deleter> mBlock;
    };

    class FontAdapter
    {
    public:
        FontAdapter() = default;
        FontAdapter(const char *path, uint8_t fontId)
            : mFont(rdpq_font_load(path)), mFontId(fontId)
        {
            if (mFont)
                rdpq_text_register_font(mFontId, mFont.get());
        }

        ~FontAdapter()
        {
            if (mFont)
                rdpq_text_unregister_font(mFontId);
        }

        FontAdapter(const FontAdapter &) = delete;
        FontAdapter &operator=(const FontAdapter &) = delete;
        FontAdapter(FontAdapter &&other) noexcept
            : mFont(std::move(other.mFont)), mFontId(other.mFontId) {}
        FontAdapter &operator=(FontAdapter &&other) noexcept
        {
            if (mFont)
                rdpq_text_unregister_font(mFontId);
            mFont = std::move(other.mFont);
            mFontId = other.mFontId;
            return *this;
        }

        rdpq_font_t *get() const noexcept { return mFont.get(); }
        explicit operator bool() const noexcept { return mFont != nullptr; }

    private:
        struct Deleter
        {
            void operator()(rdpq_font_t *p) const noexcept
            {
                if (p)
                    rdpq_font_free(p);
            }
        };

        std::unique_ptr<rdpq_font_t, Deleter> mFont;
        uint8_t mFontId{};
    };

    class SkeletonAdapter
    {
    public:
        SkeletonAdapter() = default;
        explicit SkeletonAdapter(T3DModel *model)
        {
            mSkeleton = t3d_skeleton_create(model);
        }

        ~SkeletonAdapter()
        {
            t3d_skeleton_destroy(&mSkeleton);
        }

        SkeletonAdapter(const SkeletonAdapter &) = delete;
        SkeletonAdapter &operator=(const SkeletonAdapter &) = delete;
        SkeletonAdapter(SkeletonAdapter &&other) noexcept : mSkeleton(other.mSkeleton)
        {
            other.mSkeleton = {};
        }
        SkeletonAdapter &operator=(SkeletonAdapter &&other) noexcept
        {
            if (this != &other)
            {
                t3d_skeleton_destroy(&mSkeleton);
                mSkeleton = other.mSkeleton;
                other.mSkeleton = {};
            }
            return *this;
        }

        T3DSkeleton *get() noexcept { return &mSkeleton; }
        const T3DSkeleton *get() const noexcept { return &mSkeleton; }

    private:
        T3DSkeleton mSkeleton{};
    };

    class AnimAdapter
    {
    public:
        AnimAdapter() = default;
        AnimAdapter(T3DModel *model, const char *name)
        {
            mAnim = t3d_anim_create(model, name);
        }

        ~AnimAdapter()
        {
            t3d_anim_destroy(&mAnim);
        }

        AnimAdapter(const AnimAdapter &) = delete;
        AnimAdapter &operator=(const AnimAdapter &) = delete;
        AnimAdapter(AnimAdapter &&other) noexcept : mAnim(other.mAnim)
        {
            other.mAnim = {};
        }
        AnimAdapter &operator=(AnimAdapter &&other) noexcept
        {
            if (this != &other)
            {
                t3d_anim_destroy(&mAnim);
                mAnim = other.mAnim;
                other.mAnim = {};
            }
            return *this;
        }

        T3DAnim *get() noexcept { return &mAnim; }
        const T3DAnim *get() const noexcept { return &mAnim; }

        void attach(SkeletonAdapter &skeleton)
        {
            t3d_anim_attach(&mAnim, skeleton.get());
        }

    private:
        T3DAnim mAnim{};
    };
}