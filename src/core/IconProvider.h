// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <foundation/NonCopyable.h>

#include <QtGui/QIcon>
#include <memory>

class IconPack;

class IconProvider : private NonCopyable {
 private:
  IconProvider();

  ~IconProvider();

 public:
  static IconProvider& getInstance();

  void setIconPack(std::unique_ptr<IconPack> pack);

  void addIconPack(std::unique_ptr<IconPack> pack);

  QIcon getIcon(const QString& iconKey) const;

 private:
  std::unique_ptr<IconPack> m_iconPack;
};
