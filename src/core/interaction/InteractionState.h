// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
// NOLINTBEGIN(cppcoreguidelines-special-member-functions, misc-non-private-member-variables-in-classes, cppcoreguidelines-c-copy-assignment-signature, misc-unconventional-assign-operator)
#include <QCursor>
#include <QString>
#include <boost/intrusive/list.hpp>

#include "NonCopyable.h"
#include "Proximity.h"

class Proximity;

class InteractionState : private NonCopyable {
 public:
  class Captor : public boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>> {
    friend class InteractionState;

   private:
    using BaseHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
    struct CopyHelper {
      Captor* captor;

      explicit CopyHelper(Captor* cap) : captor(cap) {}
    };

   public:
    Captor() = default;

    // NOLINTNEXTLINE(bugprone-copy-constructor-init)
    Captor(Captor& other) {
      swap_nodes(other);
    }

    explicit Captor(CopyHelper other) {
      swap_nodes(*other.captor);
    }

    Captor& operator=(Captor& other);

    Captor& operator=(CopyHelper other);

    explicit operator CopyHelper() { return CopyHelper(this); }

    void release() { unlink(); }

    [[nodiscard]] const QCursor& proximityCursor() const { return m_proximityCursor; }

    void setProximityCursor(const QCursor& cursor) { m_proximityCursor = cursor; }

    [[nodiscard]] const QCursor& interactionCursor() const { return m_interactionCursor; }

    void setInteractionCursor(const QCursor& cursor) { m_interactionCursor = cursor; }

    [[nodiscard]] const QString& proximityStatusTip() const { return m_proximityStatusTip; }

    void setProximityStatusTip(const QString& tip) { m_proximityStatusTip = tip; }

    [[nodiscard]] const QString& interactionStatusTip() const { return m_interactionStatusTip; }

    void setInteractionStatusTip(const QString& tip) { m_interactionStatusTip = tip; }

    [[nodiscard]] const QString& interactionOrProximityStatusTip() const {
      return m_interactionStatusTip.isNull() ? m_proximityStatusTip : m_interactionStatusTip;
    }

   private:
    QCursor m_proximityCursor;
    QCursor m_interactionCursor;
    QString m_proximityStatusTip;
    QString m_interactionStatusTip;
  };


  InteractionState();

  void capture(Captor& captor);

  [[nodiscard]] bool captured() const { return !m_captorList.empty(); }

  [[nodiscard]] bool capturedBy(const Captor& captor) const;

  void resetProximity();

  void updateProximity(Captor& captor,
                       const Proximity& proximity,
                       int priority = 0,
                       Proximity proximityThreshold = Proximity());

  [[nodiscard]] bool proximityLeader(const Captor& captor) const;

  [[nodiscard]] const Proximity& proximityThreshold() const { return m_proximityThreshold; }

  [[nodiscard]] QCursor cursor() const;

  [[nodiscard]] QString statusTip() const;

  [[nodiscard]] const QString& defaultStatusTip() const { return m_defaultStatusTip; }

  void setDefaultStatusTip(const QString& statusTip) { m_defaultStatusTip = statusTip; }

  [[nodiscard]] bool redrawRequested() const { return m_redrawRequested; }

  void setRedrawRequested(bool requested) { m_redrawRequested = requested; }

 private:
  using CaptorList = boost::intrusive::list<Captor, boost::intrusive::constant_time_size<false>>;

  /**
   * Returns true if the provided proximity is better than the stored one.
   */
  [[nodiscard]] bool betterProximity(const Proximity& proximity, int priority) const;

  QString m_defaultStatusTip;
  CaptorList m_captorList;
  CaptorList m_proximityLeader;
  Proximity m_bestProximity;
  Proximity m_proximityThreshold;
  int m_bestProximityPriority;
  bool m_redrawRequested;
};
// NOLINTEND(cppcoreguidelines-special-member-functions, misc-non-private-member-variables-in-classes, cppcoreguidelines-c-copy-assignment-signature, misc-unconventional-assign-operator)
