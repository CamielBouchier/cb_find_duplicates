-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Contains 'control_center_4'"
description = "select those containing 'control_center_4' in path"

function select(files, times, size)
  -- files : table of filenames.
  -- times : corresponding table of modification times. 
  -- size  : filesize.
  local selected = {}
  for i=1,#files do
    selected[i] = (nil ~= string.find(files[i],"control_center_4"))
  end
  all_selected = true
  for i=1,#files do
    if not selected[i] then
      all_selected = false
      break
    end
  end
  if all_selected then
    selected[1] = false
  end
  return selected
end

-------------------------------------------------------------------------------
