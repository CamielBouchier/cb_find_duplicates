-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Oldest"
description = "Select oldest from group"

function select(files, times, size)
  -- files : table of filenames.
  -- times : corresponding table of modification times. 
  -- size  : filesize.
  local selected = {}
  pivot = 1
  for i=2, #files do
    if times[i] > times[pivot] then
      pivot = i
    end
  end
  for i=1, #files do
    selected[i] = (i ~= pivot)
  end
  return selected
end

-------------------------------------------------------------------------------
