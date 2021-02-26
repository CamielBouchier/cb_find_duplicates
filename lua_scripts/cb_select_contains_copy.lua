-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Contains 'Copy'"
description = "Select those containing 'Copy' in path"

function select(files, mtimes, ctimes, inodes, size)
  -- files : table of filenames.
  -- mtimes : corresponding table of modification times. 
  -- ctimes : corresponding table of creation times. 
  -- inodes : corresponding table of (fake) inodes. 
  -- size  : filesize.
  local selected = {}
  for i=1, #files do
    selected[i] = (nil ~= string.find(files[i], "Copy"))
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
