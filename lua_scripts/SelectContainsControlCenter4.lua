-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

Title       = "Contains 'ControlCenter4'"
Description = "Select those containing 'ControlCenter4' in path"

function Select(Files,Times,Size)
  -- Files : table of filenames.
  -- Times : corresponding table of modification times. 
  -- Size  : filesize.
  local Selected = {}
  for i=1,#Files do
    Selected[i] = (nil ~= string.find(Files[i],"ControlCenter4"))
  end
  AllSelected = true
  for i=1,#Files do
    if not Selected[i] then
      AllSelected = false
      break
    end
  end
  if AllSelected then
    Selected[1] = false
  end
  return Selected
end

-------------------------------------------------------------------------------
