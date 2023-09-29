/**
 * rocky c++
 * Copyright 2023 Pelican Mapping
 * MIT License
 */
#pragma once
#include <rocky_vsg/Common.h>
#include <rocky/Math.h>

#include <vsg/vk/State.h>
#include <vsg/nodes/Transform.h>
#include <vsg/state/ViewDependentState.h>

namespace ROCKY_NAMESPACE
{
    /**
    * Transform that lets you set an object's size in pixels rather than
    * scene units (e.g. meters). Good for text or other billboarded
    * screen-space geometry.
    *
    * This won't work great for multi-threaded record traversals..
    */
    class ROCKY_VSG_EXPORT PixelScaleTransform : public vsg::Inherit<vsg::Transform, PixelScaleTransform>
    {
    public:
        //! Whether to undo any rotation found in the origial model view matrix;
        //! This will effectively billboard the geometry.
        bool unrotate = false;

        void accept(vsg::RecordTraversal& rt) const override
        {
            // Calculate the scale factor that will scale geometry from pixel space to model space
            auto& state = *rt.getState();
            auto& viewport = state._commandBuffer->viewDependentState->viewportData->at(0);
            double d = state.lodDistance(vsg::dsphere(0.0, 0.0, 0.0, 0.5)) / viewport[3]; // vp height
            matrix = vsg::scale(d);

            if (unrotate)
            {
                vsg::dquat rotation;
                auto& mv = state.modelviewMatrixStack.top();
                get_rotation_from_matrix(mv, rotation);
                matrix = matrix * vsg::rotate(vsg::inverse(rotation));
            }
            rt.apply(*this);
        };

        vsg::dmat4 transform(const vsg::dmat4& mv) const override
        {
            return mv * matrix;
        }


    private:
        mutable vsg::dmat4 matrix;
    };
}
