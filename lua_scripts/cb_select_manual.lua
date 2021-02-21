-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Manual"
description = "Select manually"

function select(files, times, size)
  -- files : table of filenames.
  -- times : corresponding table of modification times. 
  -- size  : filesize.
  local selected = {}
  for i=1, #files do
    selected[i] = false
  end
  return selected
end

-------------------------------------------------------------------------------
