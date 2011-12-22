/*
 * Copyright (c) 2011, Dorian Scholz, TU Darmstadt
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the TU Darmstadt nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "rviz.h"

#include <pluginlib/class_list_macros.h>
#include <OGRE/OgreLogManager.h>

#include <QCloseEvent>

namespace rosgui_rviz {

RViz::RViz()
  : rosgui_roscpp::Plugin()
  , context_(0)
  , widget_(0)
{
  setObjectName("RViz");
}

void RViz::initPlugin(rosgui_cpp::PluginContext& context)
{
  context_ = &context;
  Ogre::LogManager* log_manager = new Ogre::LogManager();
  log_manager->createLog("Ogre.log", false, false, false);

  widget_ = new rviz::VisualizationFrame();
  // TODO: pass arguments
  widget_->initialize("", "", "", "", false);

  widget_->setWindowTitle("RViz");
  if (context.serial_number() != 1)
  {
    widget_->setWindowTitle(widget_->windowTitle() + " (" + QString::number(context.serial_number()) + ")");
  }
  context.add_widget(widget_, Qt::RightDockWidgetArea);

  // trigger deleteLater for plugin when widget or frame is closed
  widget_->installEventFilter(this);
}

bool RViz::eventFilter(QObject* watched, QEvent* event)
{
  if (watched == widget_ && event->type() == QEvent::Close)
  {
    event->ignore();
    context_->close_plugin();
    return true;
  }

  return QObject::eventFilter(watched, event);
}

void RViz::saveSettings(rosgui_cpp::Settings& global_settings, rosgui_cpp::Settings& perspective_settings)
{
}

void RViz::restoreSettings(rosgui_cpp::Settings& global_settings, rosgui_cpp::Settings& perspective_settings)
{
}

}

PLUGINLIB_DECLARE_CLASS(rosgui_rviz, RViz, rosgui_rviz::RViz, rosgui_roscpp::Plugin)
