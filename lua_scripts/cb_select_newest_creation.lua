-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Newest (creation time)"
description = "Select newest (creation time) from group"

function select(files, mtimes, ctimes, inodes, size)
  -- files : table of filenames.
  -- mtimes : corresponding table of modification times. 
  -- ctimes : corresponding table of creation times. 
  -- inodes : corresponding table of (fake) inodes. 
  -- size  : filesize.
  local selected = {}
  pivot = 1
  for i=2, #files do
    if ctimes[i] < ctimes[pivot] then
      pivot = i
    end
  end
  for i=1, #files do
    selected[i] = (i ~= pivot)
  end
  return selected
end

-------------------------------------------------------------------------------
