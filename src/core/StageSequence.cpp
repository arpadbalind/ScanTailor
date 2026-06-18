// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "StageSequence.h"

#include <ranges>
#include <optional>

#include "ProjectPages.h"

StageSequence::StageSequence(const std::shared_ptr<ProjectPages>& pages,
                             const PageSelectionAccessor& pageSelectionAccessor)
    : m_fixOrientationFilter(std::make_shared<fix_orientation::Filter>(pageSelectionAccessor)),
      m_pageSplitFilter(std::make_shared<page_split::Filter>(pages, pageSelectionAccessor)),
      m_deskewFilter(std::make_shared<deskew::Filter>(pageSelectionAccessor)),
      m_selectContentFilter(std::make_shared<select_content::Filter>(pageSelectionAccessor)),
      m_pageLayoutFilter(std::make_shared<page_layout::Filter>(pages, pageSelectionAccessor)),
      m_outputFilter(std::make_shared<output::Filter>(pageSelectionAccessor)) {
  m_fixOrientationFilterIdx = m_filters.size();
  m_filters.emplace_back(m_fixOrientationFilter);

  m_pageSplitFilterIdx = m_filters.size();
  m_filters.emplace_back(m_pageSplitFilter);

  m_deskewFilterIdx = m_filters.size();
  m_filters.emplace_back(m_deskewFilter);

  m_selectContentFilterIdx = m_filters.size();
  m_filters.emplace_back(m_selectContentFilter);

  m_pageLayoutFilterIdx = m_filters.size();
  m_filters.emplace_back(m_pageLayoutFilter);

  m_outputFilterIdx = m_filters.size();
  m_filters.emplace_back(m_outputFilter);
}

void StageSequence::performRelinking(const AbstractRelinker& relinker) {
  for (FilterPtr& filter : m_filters) {
    filter->performRelinking(relinker);
  }
}

std::optional<std::size_t> StageSequence::findFilter(const FilterPtr& filter) const
{
  auto it = std::ranges::find(m_filters, filter);

  if (it == m_filters.end())
    return std::nullopt;

  return static_cast<std::size_t>(std::distance(m_filters.begin(), it));
}
