-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Newest > 10MB"
description = "Select newest from group, but leave < 10MB untouched."

function select(files, times, size)
  -- files : table of filenames.
  -- times : corresponding table of modification times. 
  -- size  : filesize.
  local selected = {}
  pivot = 1
  for i=2, #files do
    if times[i] < times[pivot] then
      pivot = i
    end
  end
  for i=1, #files do
    selected[i] = (i ~= pivot) and (size > 10485760)
  end
  return selected
end

-------------------------------------------------------------------------------
