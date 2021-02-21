-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Contains 'build'"
description = "Select those containing 'buid' in path"

function select(files, times, size)
  -- files : table of filenames.
  -- times : corresponding table of modification times. 
  -- size  : filesize.
  local selected = {}
  for i=1, #files do
    selected[i] = (nil ~= string.find(files[i], "build"))
  end
  all_selected = true
  for i=1, #files do
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
