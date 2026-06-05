// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QMutex>
#include <QObject>
#include <cstddef>
#include <vector>

#include "NonCopyable.h"

class OutOfMemoryHandler : public QObject {
  Q_OBJECT
  DECLARE_NON_COPYABLE(OutOfMemoryHandler)

 public:
  static OutOfMemoryHandler& instance();

  /**
   * To be called once, before any OOM situations can occur.
   */
  void allocateEmergencyMemory(size_t bytes);

  /** May be called from any thread. */
  void handleOutOfMemorySituation();

  bool hadOutOfMemorySituation() const;

 signals:

  /** Will be dispatched from the main thread. */
  void outOfMemory();

 private:
  OutOfMemoryHandler();

  mutable QMutex m_mutex;
  std::vector<char> m_emergencyBuffer;
  bool m_hadOOM;
};
